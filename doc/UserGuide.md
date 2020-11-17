### Using the stdinCommandFacility
There is a really simple and basic implementation that comes with the package.
The stdinCommandFacility reads the available commands from a file, then one can
execute these command by typing their IDs on stdin:

    daq_application -c stdin://sourcecode/appfwk/schema/fdpc-job.json

