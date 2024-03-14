// Demo:   spectrogram.cpp
// Author: Evan Pezent (evanpezent.com)
// Date:   3/26/2021

#include "App.h"

#include <kiss_fftr.h>
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

#include <atomic>
#include <mutex>
#include <iostream>
#include <filesystem>
#include <vector>
#include <complex>

namespace ImGui {
bool SliderDouble(const char* label, double* v, double v_min, double v_max, const char* format = NULL, ImGuiSliderFlags flags = 0) {
    return SliderScalar(label, ImGuiDataType_Double, v, &v_min, &v_max, format, flags);
}
}

template <typename T>
static inline T remap(T x, T x0, T x1, T y0, T y1) { return y0 + (x - x0) * (y1 - y0) / (x1 - x0); }

template <typename T>
static inline T remap01(T x, T x0, T x1) { return (x - x0) / (x1 - x0); }

std::atomic_bool g_playing{false}; // should audio play?
std::mutex       g_mtx;            // mutex to protect decoder data

// miniaudio playback callback
void audio_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL)
        return;
    if (g_playing) {
        std::lock_guard<std::mutex> lock(g_mtx);
        ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount);
    }
    (void)pInput;
}

// Spectrogram App
struct ImSpectrogram : App {

    static constexpr int WIN_W = 640;            // window width
    static constexpr int WIN_H = 480;            // window height

    static constexpr int FS    = 44100;          // sampling rate
    static constexpr int T     = 10;             // spectrogram display period
    static constexpr int N_FFT = 1024;           // FFT size
    static constexpr int N_FRQ = N_FFT/2+1;      // FFT frequency count
    static constexpr int N_BIN = FS * T / N_FFT; // spectrogram bin count

    kiss_fftr_cfg       m_fft;                   // FFT handle
    std::complex<float> m_fft_out[N_FFT];        // FFT output buffer
    float               m_fft_frq[N_FRQ];        // FFT output frequencies

    ma_decoder         m_decoder;                // miniaudio audio file decoder
    ma_device          m_device;                 // miniaudio playback device

    std::string        m_filename;               // filename of audio file provided
    double             m_duration;               // length of audio file in seconds
    std::vector<float> m_samples;                // local copy of audio file samples
    std::vector<float> m_spectrogram;            // spectrogram matrix data

    double m_time  = 0;                          // current playback time in seconds

    double m_min_db = -25;                        // minimum spectrogram dB
    double m_max_db =  40;                        // maximum spectrogram dB

    ImSpectrogram(std::string title, int argc, char const *argv[]) :
        App(title, WIN_W,WIN_H,argc,argv),
        m_spectrogram(N_FRQ*N_BIN,0)
    {
        std::string filepath = (argc < 2) ? "aphex_twin_formula.wav" : argv[1];
        // allocate FFT memory
        m_fft = kiss_fftr_alloc(N_FFT,0,nullptr,nullptr);
        // generate FFT frequencies
        for (int f = 0; f < N_FRQ; ++f)
            m_fft_frq[f] = f * (float)FS / (float)N_FFT;
        // get filename
        std::filesystem::path p(filepath);
        m_filename = p.filename().string();
        // initialize decoder (force float, mono, 44100 Hz)
        auto decoder_cfg = ma_decoder_config_init(ma_format_f32,1,FS);
        if (ma_decoder_init_file(filepath.c_str(), &decoder_cfg, &m_decoder) != MA_SUCCESS)
            std::runtime_error("Failed to decode audio file: " + filepath);
        // read all samples to local buffer
        auto n_samples = ma_decoder_get_length_in_pcm_frames(&m_decoder);
        m_samples.resize(n_samples);
        ma_decoder_read_pcm_frames(&m_decoder, m_samples.data(), n_samples);
        ma_decoder_seek_to_pcm_frame(&m_decoder, 0);
        // compute audio file duration
        m_duration = (double)n_samples / (double)FS;
        // prewarm spectrogram
        update_spectrogram(0);
        // initialize playback device
        auto device_cfg = ma_device_config_init(ma_device_type_playback);
        device_cfg.playback.format   = m_decoder.outputFormat;
        device_cfg.playback.channels = m_decoder.outputChannels;
        device_cfg.sampleRate        = m_decoder.outputSampleRate;
        device_cfg.dataCallback      = audio_callback;
        device_cfg.pUserData         = &m_decoder;
        if (ma_device_init(NULL, &device_cfg, &m_device) != MA_SUCCESS)
           std::runtime_error("Failed to open playback device.");
        // start audio playback thread
        if (ma_device_start(&m_device) != MA_SUCCESS)
            std::runtime_error("Failed to start playback device.");
        // set colormap
        ImPlot::GetStyle().Colormap = ImPlotColormap_Plasma;
    }

    // Destructor
    ~ImSpectrogram() {
        free(m_fft);
        ma_device_uninit(&m_device);
        ma_decoder_uninit(&m_decoder);
    }

    // Update function called once per frame
    void Update() override {

        if (g_playing && m_time <= m_duration) {
            m_time += ImGui::GetIO().DeltaTime;
            m_time = std::clamp(m_time, 0.0, m_duration);
            update_spectrogram(m_time);
        }

        ImGui::SetNextWindowPos({0,0},ImGuiCond_Always);
        ImGui::SetNextWindowSize({WIN_W,WIN_H},ImGuiCond_Always);
        ImGui::Begin("ImPlot Spectrogram",nullptr,ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoTitleBar);

        ImGui::TextUnformatted(m_filename.c_str());
        ImGui::SameLine(560);
        ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
        
        if (ImGui::Button(g_playing ? ICON_FA_PAUSE : ICON_FA_PLAY)) 
            g_playing = !g_playing;
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_STEP_BACKWARD))
            seek(0);
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_STEP_FORWARD))
            seek(m_duration);
        
        ImGui::SameLine();
 
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::SliderDouble("##Scrub", &m_time, 0, m_duration, "%.3f s"))
            seek(m_time);
        const double tmin = (m_duration-T)*(m_time/m_duration);
        const double tmax = tmin + T;
        const float w  = ImGui::GetContentRegionAvail().x - 100 - ImGui::GetStyle().ItemSpacing.x;
        const float h = 320.0f;
        if (ImPlot::BeginPlot("##Plot1",ImVec2(w,h),ImPlotFlags_NoMouseText)) {
            ImPlot::SetupAxes(NULL,NULL,ImPlotAxisFlags_NoTickLabels,ImPlotAxisFlags_Lock);
            ImPlot::SetupAxisLimits(ImAxis_X1,tmin,tmax,ImGuiCond_Always);
            ImPlot::SetupAxisLimits(ImAxis_Y1,0,20);
            ImPlot::SetupAxisFormat(ImAxis_Y1,"%g kHz");
            ImPlot::PlotHeatmap("##Heat",m_spectrogram.data(),N_FRQ,N_BIN,m_min_db,m_max_db,NULL,{tmin,0},{tmax,m_fft_frq[N_FRQ-1]/1000});
            if (ImPlot::DragLineX(838492,&m_time,{1,1,1,1},1,ImPlotDragToolFlags_Delayed)) 
                seek(m_time);            
            ImPlot::EndPlot();
        }
        ImGui::SameLine();
        ImPlot::ColormapScale("##Scale",m_min_db,m_max_db,{100,h},"%g dB");
        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
            ImGui::OpenPopup("Range");
        if (ImGui::BeginPopup("Range")) {
            ImGui::SliderDouble("Max",&m_max_db,m_min_db,100);
            ImGui::SliderDouble("Min",&m_min_db,-100,m_max_db);
            ImGui::EndPopup();
        }

        ImPlot::PushStyleVar(ImPlotStyleVar_PlotMinSize,{0,0});
        if (ImPlot::BeginPlot("##Plot2",ImVec2(-1,-1),ImPlotFlags_CanvasOnly)) {
            ImPlot::SetupAxes(NULL,NULL,ImPlotAxisFlags_NoDecorations,ImPlotAxisFlags_NoDecorations);
            ImPlot::SetupAxesLimits(0,1,-1,1,ImGuiCond_Always);
            int idx = (int)((m_samples.size() - N_FFT) * (m_time/m_duration));
            idx -= idx % N_FFT;
            kiss_fftr(m_fft, &m_samples[idx], reinterpret_cast<kiss_fft_cpx*>(m_fft_out));
            auto getter1 = [](int i, void* data) {
                ImSpectrogram& spec = *(ImSpectrogram*)(data);
                double db = 20*log10(std::abs(spec.m_fft_out[i]));
                double x = remap01((double)i,0.0,(double)(N_FRQ-1));
                double y = remap(db,spec.m_min_db,spec.m_max_db,-1.0,1.0);
                return ImPlotPoint(x,y);
            };
            auto getter2 = [](int i, void*) { 
                double x = remap01((double)i,0.0,(double)(N_FRQ-1));
                return ImPlotPoint(x,-1.0);
            };
            ImPlot::SetNextFillStyle({1,1,1,0.1f});
            ImPlot::PlotShadedG("##FreqDomain",getter1,this,getter2,nullptr,N_FRQ);
            ImPlot::SetNextLineStyle(ImPlot::SampleColormap(0.8f));
            ImPlot::PlotLine("##TimeDomain",&m_samples[idx],N_FFT,1.0/(N_FFT-1));
            ImPlot::EndPlot();
        }
        ImPlot::PopStyleVar();
        ImGui::End();
    };

    // Update spectrogram matrix at playback time
    void update_spectrogram(double time) {
        double t = time / m_duration;
        int idx = (int)((m_samples.size() - N_BIN*N_FFT) * t);
        idx -= idx % N_FFT;
        for (int b = 0; b < N_BIN; ++b) {
            kiss_fftr(m_fft, &m_samples[idx], reinterpret_cast<kiss_fft_cpx*>(m_fft_out));
            for (int f = 0; f < N_FRQ; ++f)
                m_spectrogram[f*N_BIN+b] = 20*log10f(std::abs(m_fft_out[N_FRQ-1-f]));
            idx += N_FFT;
        }
    }

    // Seek to playback time
    void seek(double time) {
        m_time = std::clamp(time, 0.0, m_duration);
        if (!g_playing)
            update_spectrogram(m_time);
        double t = m_time / m_duration;
        auto frame = (int)((m_samples.size()-1) * t);
        {
            std::lock_guard<std::mutex> lock(g_mtx);
            ma_decoder_seek_to_pcm_frame(&m_decoder, frame);
        }
    }
};

// Main
int main(int argc, char const *argv[]) {
    ImSpectrogram app("ImSpectrogram",argc,argv);
    app.Run();
    return 0;
}
