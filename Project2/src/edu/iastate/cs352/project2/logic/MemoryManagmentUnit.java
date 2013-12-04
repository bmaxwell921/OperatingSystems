package edu.iastate.cs352.project2.logic;

import edu.iastate.cs352.project2.info.ThreadInfo;

/**
 * Class used to manage the shared physical memory
 * @author Brandon
 *
 */
public class MemoryManagmentUnit {
	
	/**
	 * Loads the required page into main memory using LRU if there 
	 * isn't a free frame
	 * @param address
	 * @param info
	 * @return
	 * 			the frame number the page was loaded into
	 */
	public static synchronized int loadPage(int address, ThreadInfo info) {
		// TODO
		return 0;
	}
}
