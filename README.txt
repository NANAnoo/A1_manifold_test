
# Describtion

This is a cmake project with multiple targets:

1. face2faceindex
    read in a .tri file and output a .face file,
    where .tri file is a polygen soup and .face file is a face index structure file

    the algorithmic complexity of transform is O(n)


# How To Build

1. face2faceindex:

camke --build ./ --target face2faceindex

2. 


# How To Run

1. face2faceindex：

./face2faceindex input_path out_put_path

    'input_path' is required, .tri file

    'out_put_path' is optional, default is 'CWD/$(input_object_name).face'

2.