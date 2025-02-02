set -e
if [[ "$1" == "--help" ]]; then
	echo " ╭───────────────────────────╮"
	sleep 0.05
	echo " │ Secton Temp compiler help │"
	sleep 0.1
	echo " │ ╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌ │"
	sleep 0.15
	echo " │ --help: display this help │"
	sleep 0.2
	echo " │ noClear: disable clearing │"
	sleep 0.25
	echo " │ run: compile, then run it │"
	sleep 0.3
	echo " ╰───────────────────────────╯"
	sleep 0.35
	echo "  Secton, 2025. Enjoy coding!"
else
	if [[ "$1" != "noClear" ]]; then
		clear
	fi
	
	echo -e "\033[1;34mSecton's temp compiler script"
	echo    " ╭──────────────────────╮"
	echo    " │ Compiling sources... │ 🏗"
	echo -e " ╰──────────────────────╯\033[0m"
	clang -c ./Libraries/xdg-shell-protocol.c -o ./Build/xdg.o
	clang++ ./Build/xdg.o ./Libraries/libblend2d.so ./Source/Start.C++ -I ./Libraries/blend2d/src/ -lwayland-client -o ./Build/Output.elf
	chmod +x ./Build/Output.elf

	if [[ "$1" == "run" ]]; then
		echo -e "\033[1;34m"
		echo    " ╭──────────────────────╮"
		echo    " │ Running...           │ 🧰"
		echo -e " ╰──────────────────────╯\033[0m"
		# clang++ ./Build/xdg.o ./Build/b2d.o ./Build/test.o -o ./Build/Output.elf -I ./Libraries/blend2d/src/ -lwayland-client --target=amd64 -fPIE
		cd Build
		./Output.elf
	fi
fi
