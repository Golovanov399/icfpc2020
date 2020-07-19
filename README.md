# Usage

1. Build the `destroy` file: `g++ -std=c++17 -O2 destroy.cpp -o destroy`.

2. Install `pygame` module: `pip3 install --user pygame`

3. Run `python game.py` and enjoy.

# Keymap

- mouse clicks to proceed to some location
- `f3` for load
- `f2` for save
- `escape` to discard saving/loading and proceed back to game
- arrows up and down to choose the save to load
- `enter` to load the chosen save or to save the current location

# Notes

Depending on the commit and your OS you may want to replace some `os.system(... < ... | ...)` by `sp.check_output()` calls or vice versa. It may be done with commenting some lines and uncommenting some others.
