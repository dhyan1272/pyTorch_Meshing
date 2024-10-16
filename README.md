## pyTorch_Meshing
ssh nplfen01 \
export http_proxy=http://proxy:8888 \
export https_proxy=$http_proxy \
wget https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh \
bash Miniconda3-latest-Linux-x86_64.sh -p ~/scratch/miniconda3x86 \
source ~/.bashrc \
module load gcc \
module load cuda/8.1 \
conda create --name torch-env5 python=3.9 \
conda install pytorch torchvision torchaudio pytorch-cuda=11.8 -c pytorch -c nvidia \
conda install pyg=*=*cu* -c pyg \
conda install conda-forge::torch-scatter \
conda install esri::torch-cluster \

