package edu.iastate.cs352.project2.logic;

import java.util.ArrayList;
import java.util.List;

import edu.iastate.cs352.project2.info.ThreadInfo;

/**
 * Class used to manage the shared physical memory
 * @author Brandon
 *
 */
public class MemoryManagementUnit {
	// TODO once a process terminates remove all its pages?? - Seems logical to me
	
	
	// 'Physical memory'
	private final Frame[] mainMemory;
	
	// Stack used in LRU replacement
	private final List<Integer> frameRefStack;
	
	public MemoryManagementUnit(int frames) {
		mainMemory = new Frame[frames];
		frameRefStack = new ArrayList<Integer>();
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
	
	public synchronized boolean frameIsOwnedBy(int frame, ThreadInfo info) {
		return (frame >= 0 && frame < mainMemory.length) && // Valid frame number
				mainMemory[frame] != null && mainMemory[frame].equals(info); //Actually belongs to given thread
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
