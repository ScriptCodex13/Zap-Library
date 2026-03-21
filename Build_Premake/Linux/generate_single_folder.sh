resource_dir="../../resource"
application_dir="./Project"

mkdir -p "$resource_dir"
mkdir -p "$application_dir"

mkdir -p "$resource_dir/shader"
mkdir -p "$resource_dir/textures"
mkdir -p "$resource_dir/sounds"
mkdir -p "$resource_dir/models"

cp -r "$resource_dir/shader/."   "$application_dir/shader/"
cp -r "$resource_dir/textures/." "$application_dir/textures/"
cp -r "$resource_dir/sounds/."   "$application_dir/sounds/"
cp -r "$resource_dir/models/."   "$application_dir/models/"

premake5 gmake --file=premake5_single_folder.lua

read -p "Done ! Press Enter..."

