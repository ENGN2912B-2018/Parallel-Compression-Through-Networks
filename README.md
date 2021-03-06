# Parallel Image Compression Through Networks
**Course Information:**

    ENGN 2912B - Scientific Computing in C++
    Fall 2018, Brown University
    December, 2018

**Team Information:**

    Multithreading and Socket Programming - Team A

    Algorithm Engineer:
    Yang Wang, yang_wang1@brown.edu

    Software Architect:
    Luis Aguirre, luis_aguirre@brown.edu

## Introduction
The goal of this project is to develop a parallelized image compression service that allows for image transfer through an asynchronous TCP network.

## Algorithms
We implemented the following algorithms independently:
- **Parallelized Discrete Cosine Transform (DCT)**

    We implemented a block-wise DCT and assigned each thread equal workload during compression and decompression. Parallelization was achieved using [openMP](https://www.openmp.org/).
- **Quantization**

    The standard JPEG quantization matrix and quality ratio formalism was adopted to compress information stored in the high frequency Fourier coefficients.
- **Huffman Coding**

    The coefficient matrix was scanned in a zigzag fashion(until we have obtained five zero coefficients in each block) before being Huffman encoded and vice versa (Huffman decoding then zigzag unpacking).


**Algorithm Showcase**

Following figure shows the compressed images with quality ratio of 1, 2, 5, 10, 20, 50, 90, respectively.

![alt text](https://drive.google.com/uc?export=view&id=1mCTFwoP2i-_JYhazqwzel1KjXjQ5uaH6)


## Network Programming
**Client-Server Model & Asynchronous Network**

We implemented a Client-Server model using the TCP communication protocol to transfer Huffman codes through an asynchronous network. The Client-Server model allows for simple message transfer and can be easily expanded upon to allow for varying Client and Server operations.

## Software Interface
**Functionalities**

The software GUI was developed using the [Qt5 Widgets library](https://doc.qt.io/qt-5.11/qtwidgets-index.html) and connects the other components of the project together. The main functions of the application includes:
- Load an image file within a given directory into the GUI.
- Choose a quality ratio before compression.
- Compress the file and send the Huffman codes through the network to a central server.
- Save the decoded and then decompressed file into a local directory.

**GUI Snapshot**

Following is a screen shot of the GUI, where the left image is the original PGM file and the right image is the compressed PGM file with quality ratio of 17 (adjustable within the GUI).

![alt text](https://drive.google.com/uc?export=view&id=1Z7Rzw5FhtaiJNbFLtEGJqIbqc0Bf_4w3)

## Dependencies and Build Instructions.
The project is built with [CMake](https://cmake.org/) and requires [Boost.Asio (v1.63.0)](https://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio.html) for asynchronous networking and the [Qt (v5.10.1)](https://www.qt.io/) for the user interface.

We have tested the software on the following platforms:

**Linux Ubuntu**
1. Install CMake, Boost and Qt using the `apt-get` pacakge manager:
```
sudo apt-get install cmake
sudo apt-get install libboost-all-dev
sudo apt-get install qt5-default
```
2. Build and compile the project by running the following commands after cloning this repo:
```
mkdir build
cd build
cmake -D CMAKE_CXX_COMPILER=g++ ..
make
```
3. Run `mainExecutable` created in the `build` directory.

**Brown CCV**
1. ssh into a CCV node with X11 forwarding enabled. i.e. Type in the following command:
```
ssh <account_name>@ssh.ccv.brown.edu -X
```
2. Load the Boost and Qt libraries to the current environment.
```
module load gcc/7.2
module load boost/1.63.0
module load qt/5.10.0
```
3. Build and compile the project by running the following commands after cloning this repo:
```
mkdir build
cd build
cmake -D CMAKE_CXX_COMPILER=g++ ..
make
```
4. Run `mainExecutable` located in the `build` directory.

Note that Windows does not support X11 forwarding by default, nor has this project been tested on Windows.

## Project Structure
The source code is kept in the `src` directory and split into the following subdirectories:
- `image`: The image folder contains the main Image class used for image processing (reading, writing) and compression (parallelized discrete cosine transform, quantization, zigzag scan).
- `algorithm`: The algorithm folder contains the code for our statistical analysis (MSE and PSNR analysis) and performing Huffman encoding and decoding.
- `client`: The client folder contains our representation of the client in the Client-Server model. The Client class uses Boost.Asio to allow for message sending to and from the server.
- `server`: The server folder contains our representation of the server in the Client-Server model. The Server class uses Boost.Asio to accept multiple client connections and send messages to and from the client.
- `gui`: The gui folder contains the code for the main client (GUI) application. The GUI application connects the algorithm and networking components of the project together through a user interface built with the Qt5 Library.

The overall relationship between the classes written for this project can be described by the following UML diagram:

![UML Diagram](https://drive.google.com/uc?export=view&id=1o2gVhemj3gtdfGjETxDOordQ46kJHRVc)

Note that the relationship between these classes and our external dependencies are not fully described in the above diagram.

## Results & Analysis
Following figure shows the performance of our compression algorithm in terms of error analysis, compression analysis and latency analysis. Image used for these analysis was `dog.binary.pgm` in the `image` folder.

![Statistical Results](https://drive.google.com/uc?export=view&id=1K7uSsTgzb6Z3fiSpthnTFwYoeK00K_1k)

**Error Analysis**

The left image in the figure above shows the mean squared error (MSE) and peak signal-to-noise ratio (PSNR) in pixel intensities for images compressed with various quality ratios. As the quality ratio increases, the MSE decreases and the PSNR increases, as expected.

**Compression Analysis**

The right image shows the dct file size (i.e the file size after we performed dct, quantization and zigzag scan), the Huffman file size (i.e. sum of Huffman bit streams representing each coefficient) and the compression ratio (original file size divided by Huffman file size) for different quality ratios. The important point here is that the compression ratio grows almost exponentially at low quality ratios but it is still bounded and depends on the specific quantization matrix used. (Think about the case of maximum compression, where we only need to use the average intensity(1 coefficient) to represent each block.)

**Latency Analysis**

By parallelizing the forward and inverse dct algorithms (which are both quadratic in (m*n) where m is the height of the block and n is the width of the block), the latency decreases linearly with respective to the number of threads set using [openMP](https://www.openmp.org/).


## Future Work
- Extend the application to support multiple file formats, such as PNG and JPEG.
- Adopt an adaptive discrete cosine transform by introducing machine learning into the image compression.
- Implement a peer-to-peer network model in order to allow for compressed image sharing.

## Author Contributions
**Luis Aguirre**:

As the software architect, Luis was responsible for building and testing of the client-server model and the asynchronous network model. He also implemented the GUI application of the software and combined each component of the project to work together in the final application. Specifically, he wrote the source code in the `client`, `server`, and `gui` namespaces.

**Yang Wang**:

As the algorithm engineer, Yang was responsible for designing, implementing and testing the image processing and image compression algorithms. Specifically, he wrote the source code for `image` class, `statistical analysis` class and `Huffman coding` class. He also tested the software on different platforms(Ubuntu and Brown CCV), and he analyzed and quantified the performance of the algorithms (space and time complexities, compression ratio and image quality trade off, sequential compression analysis etc.).
