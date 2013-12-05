package edu.iastate.cs352.project2.logic;

import edu.iastate.cs352.project2.info.ThreadInfo;

/**
 * Class used to manage the shared physical memory
 * @author Brandon
 *
 */
public class MemoryManagementUnit {
	
	public MemoryManagementUnit(int frames) {
		
	}
	
	/**
	 * Loads the required page into main memory using LRU if there 
	 * isn't a free frame
	 * @param address
	 * @param info
	 * @return
	 * 			the frame number the page was loaded into
	 */
	public synchronized int loadPage(int address, ThreadInfo info) {
		// TODO
		return 0;
	}
	
	/**
	 * Inner class used for physical main memory.
	 * We keep track of which process owns each frame
	 * that way when one process swaps out the frame of another
	 * process, that other frame can know
	 * @author Brandon
	 *
	 */
	private class Frame {
		private ThreadInfo owningThread;
		
		public Frame(ThreadInfo info) {
			this.owningThread = info;
		}
	}
}
