# cmdlib Interfaces for commanded objects

### Building and running examples:

* create a software work area
  * see https://github.com/DUNE-DAQ/appfwk/wiki/Compiling-and-running
* clone this repo
  * `cd <MyTopDir>`
  * `git clone https://github.com/DUNE-DAQ/cmdlib.git`)
* build the software
  * `. ./setup_build_environment`
  * `./build_daq_software.sh --clean --install --pkgname cmdlib`
* run the demos in another shell
  * `. ./setup_runtime_environment`
  * `cmdlib_test_dummy_app`
  * `cmdlib_test_stdin_app`
    * type commands (test a slow command, using "asd")
