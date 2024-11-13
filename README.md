# `C++` Configuration File Reader

`TConf` is a simple C++ class for reading and managing configuration files.
It supports flags, numerical values (integers, floats), strings, and lists, and allows for recursive inclusion of additional configuration files.


## Features

- Read configuration items from a file, such as integers, floats, strings, and lists.
- Supports flags and comments in the configuration file.
- Automatically handles multiple configuration files via the include directive.
- Offers an echo mode for debugging and printing the configuration content.


## Installation

To use this utility in your project:

- Clone this repository.
- Include `TConf.h` in your project.
- Compile `TConf.cpp` along with your main program.

## Configuration File Format

The configuration file is a plain text file with the following format:

- Comments: Start lines with `#` to add comments. (In practice, any line that is not recognized as a configuration elements is ignore.)
- Flags: `flag flag_name`
- Integers: `int name = value`
- Floats: `float name = value`
- Strings: `string name = "value"`
- Lists: Define lists using the syntax:
    ```c
    int list name = {
    1,
    2,
    3
    }
    ```
    Use the same structure for `float` or `string` lists.

## Methods

- `int ReadInt(const char* name, const int def = 0);`: Reads the integer value from the key `name`; if not found, returns `def`.
- `float ReadFloat(const char* name, const float def = 0.0);`: Reads the floating point value from the key `name`; if not found, returns `def`.
- `const char* ReadString(const char* name, const char* def = "");`: Reads the string value from the key `name`; if not found, returns `def`. (:warning: The `char*` array is your responsibility to `delete` when not needed anymore.)
- `int ReadFlag(const char* name);` : Reads wether the flag value under the key `name` is present. If not found, return `False`.
- `int ReadListInt(const char* name, const int idx, const int def = 0);`: Returns the `idx`-th element in a the list of integers under the key `name`. If not found (wether because the list does not exists, or it ends before `idx`), returns `def`.
- `float ReadListFloat(const char* name, const int idx, const float def = 0.0);`: Returns the `idx`-th element in a the list of floats under the key `name`. If not found (wether because the list does not exists, or it ends before `idx`), returns `def`.
- `const char* ReadListString(const char* name, const int idx, const char* def = "");`: Returns the `idx`-th element in a the list of strings under the key `name`. If not found (wether because the list does not exists, or it ends before `idx`), returns `def`.
- `include "filename"`: adds the content of `filename` to the configuration.

Note: the if several keys with the same name exists in the configuration, the first one is returned.

Note (2): there is no check forbiding the same key name to be used for `int`, `float`, ...
However, it's is highly risky to do so.

Note (3): The functions rely heavily on `sscanf`, so the syntax is rather strict.


## Example Usage

```cpp

#include "TConf.h"

int main() {
    TConf conf("config.txt", 1);  // Initialize with a file and enable echo

    int value = conf.ReadInt("someInt", 5);  // Reads an integer with default 5
    float pi = conf.ReadFloat("pi", 3.14);  // Reads a float with default 3.14
    const char* name = conf.ReadString("name", "default_name");  // Reads a string

    return 0;
}
```


## License

This project is licensed under the [CeCILL-C FREE SOFTWARE LICENSE AGREEMENT](https://cecill.info/licences/Licence_CeCILL-C_V1-en.html). See [LICENSE.txt](LICENSE.txt) for more.

## Contributing

Contributions are welcome! Please submit a pull request for any improvements or bug fixes.
