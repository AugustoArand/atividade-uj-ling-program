# Contact Management System - Tests

This directory contains test scripts and utilities for the Contact Management System.

## Test Files

### test_suite.sh
Basic functional test suite that verifies:
- Project compilation
- File structure
- Header guards
- Memory management

## Running Tests

Make the test script executable and run it:

```bash
chmod +x tests/test_suite.sh
./tests/test_suite.sh
```

## Test Coverage

The test suite covers:
1. **Compilation Test**: Verifies the project builds successfully
2. **Binary Test**: Checks if the executable is created
3. **Structure Test**: Validates all required source files exist
4. **Data Directory Test**: Ensures data directory is available
5. **Header Guards Test**: Checks proper header file protection
6. **Memory Safety Test**: Verifies memory allocation/deallocation functions

## Adding New Tests

To add new tests:
1. Create a new test function in `test_suite.sh`
2. Follow the pattern of existing tests
3. Use `print_result` function to report pass/fail status
4. Update this README with test description

## Future Enhancements

- Unit tests for individual functions
- Integration tests for complete workflows
- Performance benchmarks
- Memory leak detection with valgrind
- Code coverage reports
