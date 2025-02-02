# Blend2D on Wayland
To build a program, run `./Compiler.sh`
To run a program, you must be at "Build" directory, then run "Output.elf"
## Dependencies
`wayland` `wayland-protocols` `clang` `blend2d`
## Questions answered
### What's with the structure of code?
This was initially the starting point exclusively for NieboUI, my work-in-progress interface library, but after all the pain of trying to mix Wayland and Blend2D, i couldn't resist but publish the template, so that everyone did not have the same pain i had.
### Why did you made your own compiler script?
Because i find creating them easier than making CMake's or Makefiles.
### Windows support?
**No.**
## Template preview
![image](https://github.com/user-attachments/assets/7ca3aad3-26ff-4f24-97b2-e48a409f6a71)
