## Commands
Commands are JSON objects. They have a signature of having an `id` string element, and a `data` object which is a user defined custom parameter list. For an example, please have a look on the `-job.json` files in appfwk. For better names, these field might be renamed in the future. 

## CommandedObject
Commanded objects are meant to implement the `CommandedObject` interface from this library. They need to implement a single function, which is `execute`, and it's responsible to process the command objects.

## CommandFacility
This base class is responsible to provide an abstract 

### Using the stdinCommandFacility
There is a really simple and basic implementation that comes with the package.
The stdinCommandFacility reads the available commands from a file, then one can
execute these command by typing their IDs on stdin:

    daq_application -c stdin://sourcecode/appfwk/schema/fdpc-job.json

