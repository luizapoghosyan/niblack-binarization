# Niblack Binarization

## Overview

This project implements **Niblack's binarization algorithm**, which converts a grayscale image into a binary image using local adaptive thresholding. The algorithm calculates a dynamic threshold for each pixel based on the mean and standard deviation of the surrounding neighborhood.

### **Time Complexity**

This implementation uses **integral images**, allowing efficient computation of mean and standard deviation in **O(1)** time for each pixel. Since every pixel is processed once, the overall **time complexity is O(N)**, where **N = width × height** (total number of pixels).

## Installation

### **1. Clone the Repository**

```sh
git clone https://github.com/YOUR-USERNAME/niblack-binarization.git
cd niblack-binarization
```

### **2. Install Dependencies**

#### macOS/Linux (Homebrew):

```sh
brew install cmake libpng
```

#### Ubuntu/Debian:

```sh
sudo apt install cmake libpng-dev 
```

### **3. Build the Project**

```sh
mkdir build
cd build
cmake ..
make
```

## Usage

To apply Niblack binarization to an image:

```sh
./niblack <input_image> <output_image> <window_size> <stdev_coeff>
```

### **Example:**

```sh
./niblack input.png output.png 15 -0.2
```

- `input.png` → Input grayscale image.
- `output.png` → Output binary image.
- `15` → Window size (odd number, e.g., 15x15 region around each pixel).
- `-0.2` → Standard deviation coefficient (`k`).
