# Blend2D on Wayland
To build a program, run `./Compiler.sh`
To run a program, you must be at "Build" directory, then run "Output.elf"
## Dependencies
[`Wayland`](https://wayland.freedesktop.org/) `Wayland Protocols` [`Blend2D`](https://blend2d.com/)

You will also have to install Clang because that's the compiler that this project's compiler script uses. It should be easy to replace `clang++` with `g++`.
## Questions answered
### What's with the structure of code?
This was initially the starting point exclusively for NieboUI, my work-in-progress interface library, but after all the pain of trying to mix Wayland and Blend2D, i couldn't resist but publish the template, so that everyone did not have the same pain i had.
### Why did you made your own compiler script?
Because i find creating them easier than making CMake's or Makefiles.

> [!NOTE]
> I might rewrite it to CMake because writing CMakeLists are not as hard as i thought it would be.
### What license does this template licensed under?
Nothing; it's public domain. I believe that code templates should be under public domain.
### Are there any AI code in here?
If i recall correctly, yes. It might be the code behind working with Wayland.

Why did i ask AI to write a Wayland code? Because writing a simple application for it is a pain in the ass. **There's no excuse for making it hard.**
## Template preview
![image](https://github.com/user-attachments/assets/7ca3aad3-26ff-4f24-97b2-e48a409f6a71)
