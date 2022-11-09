
# Describtion

Thank you for reading me ~

This is a cmake project with three targets:
    I give describtion and algorithmic complexity of each program here
    And if you wanna check how the complexity comes out, 
    Please and welcome to go to my source code!
    There are algorithmic complexity analysis in my comments
------------------------------------------------------------------
1. face2faceindex
    read in a .tri file and output a .face file,
    where .tri file is a polygen soup and .face file is a face index structure file

    the algorithmic complexity of transform is O(n)

------------------------------------------------------------------
2. faceindex2directededge
    read in a .face file and output a .diredge file,
    where .face file is a face index structure file and .diredge file is a directed-edge structure file

    the algorithmic complexity of transform is O(n)

------------------------------------------------------------------
3. manifoldchecker 
    read in a .tri or .face file and detect whether the object is a manifold or not
    The result will be output in terminal
    If the object is a manifold, I will output the Genus of it like this:

        (commandline) $ ./build/manifoldchecker ./handout_models/horse.tri

        ---------------------------
        The object is manifold :)
        Genus of The object is xxx
        ---------------------------

    Otherwise, I will output the error message of what make it fails to be a manifold like this:

        (commandline) $ ./build/manifoldchecker ./handout_models/tetrahedron_bad.tri

        ------------------------------------------------------------------------
        [Uncontinous face error] The object has some faces that are uncontinuous, 
        Half Edges : 1 8 
        don't have opposite edge.

        [Pinch edges error] The object has pinch edges :
        Half-Edges : 
        0 9 
        are duplicated that represent the same half edge:
        vertex[2]-->vertex[0]

        Half-Edges : 
        6 11 
        are duplicated that represent the same half edge:
        vertex[3]-->vertex[2]

        The object is not a manifold :(
        


# How To Build

------------------------------------------------------------------------
0. Create build folder and configure cmake
$ mkdir build && cd build
$ cmake ..

------------------------------------------------------------------------
1. build face2faceindex:

$ cmake --build ./ --target face2faceindex

    And you will get the face2faceindex program in folder 'build'

------------------------------------------------------------------------
2. build faceindex2directededge:

$ cmake --build ./ --target faceindex2directededge

    And you will get the faceindex2directededge program in folder 'build'

------------------------------------------------------------------------
3. build manifoldchecker:

$ cmake --build ./ --target manifoldchecker

    And you will get the manifoldchecker program in folder 'build'


# How To Run

------------------------------------------------------------------------
1. face2faceindex：

$ ./face2faceindex input_path out_put_path

    'input_path' is required, .tri file

    'out_put_path' is optional:
        a. default is 'CWD/$(input_object_name).face'
        b. file_path: a_path/xxx.face, save into this file
        c. dir_path: a_path/, save into file a_path/$(input_object_name).face

------------------------------------------------------------------------
2. faceindex2directededge

$ ./faceindex2directededge input_path out_put_path

    'input_path' is required, .face file
    
    'out_put_path' is optional:
        a. default is 'CWD/$(input_object_name).diredge'
        b. file_path: a_path/xxx.diredge, save into this file
        c. dir_path: a_path/, save into file a_path/$(input_object_name).diredge

------------------------------------------------------------------------
3. manifoldchecker

$ ./manifoldchecker input_path

    'input_path' is required, .tri or .face file
