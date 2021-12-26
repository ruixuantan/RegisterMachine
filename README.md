# Register Machine

## How to run files
1. `cd` into the `build` directory first.
1. Compile:
    ```sh
    cmake --build .
    ```
1. Run makefile:
    ```sh
    make all
    ```
1. Run tests:
    ```sh
    ./tests/RegisterMachine_tests
    ```
1. Run the register machine:
    ```sh
    ./src/RegisterMachine_run ../<program_file> <args>
    ```

Note that the program file should be in the root directory.