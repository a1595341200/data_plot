cd imgui
###
 # @Author: error: git config user.name & please set dead value or install git 1595341200@qq.com
 # @Date: 2024-03-14 16:47:27
 # @LastEditors: error: git config user.name & please set dead value or install git 1595341200@qq.com
 # @LastEditTime: 2024-03-14 16:54:05
 # @FilePath: /cplusplus/submodule/data_plot/build.sh
 # @Description: 
 # 
 # Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
### 
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
echo "脚本所在的目录是: $DIR"
cd ${DIR}
echo `pwd`
rm -rf build_install
rm -rf build

cd imgui
rm -rf build
mkdir build
cd build
cmake ..
make
make install

cd ../../
cd implot
rm -rf build
mkdir build
cd build
cmake ..
make 
make install

cd ../../
cd implot_demos
rm -rf build
mkdir build
cd build
cmake ..
make
make install
cd ../../
mkdir build
cd build
cmake ..
make
