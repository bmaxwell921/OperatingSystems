package edu.iastate.cs352.project2.info;

/**
 * Boxes up the process id for a given address
 * @author Brandon
 *
 */
public class ThreadInfo {

	private int processId;
	
	public ThreadInfo(int processId) {
		this.processId = processId;
	}
	
	public int getProcessId() {
		return this.processId;
	}
}
