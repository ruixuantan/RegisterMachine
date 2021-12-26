# Register Machine

## How to run files
1. To build the project, run:
    ```sh
    cmake -S <current_dir> -B <current_dir>/build
    cmake --build <current_dir>/build 
    ```
1. Then, `cd` into the `build` directory.
1. Run tests:
    ```sh
    ./tests/RegisterMachine_tests
    ```
1. Run the register machine:
    ```sh
    ./src/RegisterMachine_run ../<program_file> <args>
    ```

Note that the `program_file` should be in the root directory.