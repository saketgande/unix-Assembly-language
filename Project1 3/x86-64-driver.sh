#!/bin/bash


# Check if a file name is given
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <filename.calc>"
    exit 1
fi

# Extract the file name without the extension
filename=$(basename -- "$1")
extension="${filename##*.}"
filename="${filename%.*}"

# Step 1: Write the prologue
prologue="
.data
a: .quad 0
b: .quad 0
c: .quad 0
d: .quad 0
e: .quad 0
f: .quad 0
g: .quad 0
h: .quad 0
i: .quad 0
j: .quad 0
k: .quad 0
l: .quad 0
m: .quad 0
n: .quad 0
o: .quad 0
p: .quad 0
q: .quad 0
r: .quad 0
s: .quad 0
t: .quad 0
u: .quad 0
v: .quad 0
w: .quad 0
x: .quad 0
y: .quad 0
z: .quad 0
.LC0: .string \"%d\\n\"

.text
.global main
main:
"

# Step 2: Run the compiler and capture the output
compiler_output=$( ./bin/calc3i.exe < "$1")
# echo "$compiler_output"


# Step 3: Write the epilogue
epilogue="
    movq \$60, %rax  # syscall number for exit
    xorq %rdi, %rdi  # status 0
    syscall
"

# Step 4: Write everything to the .s file
echo "$prologue$compiler_output$epilogue" > ./bin/"$filename.s"

# Step 5: Call gcc to compile and link the .s file into an executable
if gcc -no-pie -o ./bin/"$filename" ./bin/"$filename.s"; then
    echo "Compilation complete. Executable file: $filename"
else
    echo "Compilation failed."
fi

./bin/"$filename"

