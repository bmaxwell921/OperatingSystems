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

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + processId;
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		ThreadInfo other = (ThreadInfo) obj;
		if (processId != other.processId)
			return false;
		return true;
	}
}
