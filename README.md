# cmdlib - Interfaces for commanded objects

### Building and running examples:

* create a software work area
  * see https://github.com/DUNE-DAQ/appfwk/wiki/Compiling-and-running-under-v1.2.1
* those instructions should already help you to clone cmdlib to the right directory
* build the software
  * `. ./setup_build_environment`
  * `./build_daq_software.sh --install`
* you can run some examples in another shell
  * `. ./setup_runtime_environment`
  * `cmdlib_test_dummy_app`
  * `cmdlib_test_stdin_app`

### Using the stdinCommandFacility
There is a really simple and basic implementation that comes with the package.
The stdinCommandFacility reads the available commands from a file, then one can
execute these command by typing their IDs on stdin:

    daq_application -c stdin://sourcecode/appfwk/schema/fdpc-job.json

