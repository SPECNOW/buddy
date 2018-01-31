// This example runs on a MSP430F169.  In order to run this example on different MSP430
// target, recompile main.c for the appropriate target, and modify the line below with the name 
// of the newly generated executable.

// CCS must be set up for the correct target (MSP430F169, or otherwise if EXECUTABLE is modified).
// You can test that your set up is correct by running CCS, and confirming that you can start a debug
// session for the correct target.  Then, exit CCS.

var PROJECT_DIR="C:/Users/Charanpreet/Documents/GitHub/buddy/firmware/Buddy_delfino/Buddy";
var EXECUTABLE="CPU1_RAM/Buddy.out";
var GCD_TO_LOOK_FOR=17;

// Import DebugServer environment and Java packages

importPackage(Packages.com.ti.debug.engine.scripting);
importPackage(Packages.com.ti.ccstudio.scripting.environment);
importPackage(Packages.java.lang);

// Global handles to the debugger
var env;
var server;
var session;

try
{
	env=ScriptingEnvironment.instance();
	server=env.getServer("DebugServer.1");
	env.setCurrentDirectory(PROJECT_DIR)
	server.setConfig("targetConfigs/TMS320F28377S.ccxml");
	session=server.openSession(".*C28.*");
}
catch(err)
{
	throw err.toString();
	throw "Could not start DebugServer.  Is the target properly configured?";
}

// Create a log file in the current directory to log script execution
env.traceBegin("tests/WatchpointsLog.xml", "tests/DefaultStylesheet.xsl");

// Connect to target and load the program
session.target.connect()
session.memory.loadProgram(EXECUTABLE);

// Remove all Breakpoints
properties=session.breakpoint.removeAll()

// For each (a, b) combination, run the target until the watch point is hit, and verify a and b
try
{
    env.traceWrite("Running!");
	session.target.run();
} 
catch(err)
{
	env.traceWrite("An error was detected while running the script.");
	env.traceWrite("The error is as follows: ");
	env.traceWrite("--");
	env.traceWrite(err);
	env.traceWrite("--");
	env.traceWrite("Please ensure that the target and this script are set up correctly.");
}

// End session, since the program is done
server.stop();
// session.terminate();

// Close our Log File
env.traceEnd();
