package edu.iastate.cs352.project2.logic;

import java.util.ArrayList;
import java.util.List;

import edu.iastate.cs352.project2.info.ThreadInfo;
import edu.iastate.cs352.project2.logging.VirtualMemLogger;

/**
 * Class used to manage the shared physical memory
 * @author Brandon
 *
 */
public class MemoryManagementUnit {
	// TODO once a process terminates remove all its pages?? - Seems logical to me. May not have to since 
	// they'll eventually be replaced anyway
	private static final int NO_FREE = -1;
	private static final int STACK_BOTTOM = 0;
	
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
		int freeFrame = findFreeFrame();
		
		// Success case
		if (freeFrame != NO_FREE) {
			VirtualMemLogger.logFoundFrame(info, freeFrame);
			return freeFrame;
		}
		
		// get the frame to replace
		int replaceFrame = getVictim();
		
		VirtualMemLogger.logReplaceFrame(info, replaceFrame);
		
		// 'swap' it in
		mainMemory[replaceFrame] = new Frame(info);
		
		// Make sure the replaced frame doesn't get removed again soon
		updateFrameReferenceStack(replaceFrame);
		
		return replaceFrame;
	}
	
	public synchronized boolean frameIsOwnedBy(int frame, ThreadInfo info) {
		
		boolean owned = (frame >= 0 && frame < mainMemory.length) && // Valid frame number
				mainMemory[frame] != null && mainMemory[frame].equals(info); //Actually belongs to given thread
		
		// Someone successfully accessed this frame
		if (owned) {
			updateFrameReferenceStack(frame);
		}
		
		return owned;
	}
	
	/*
	 * ========================================================================================================
	 * Main Memory methods
	 * ========================================================================================================
	 */
	private int findFreeFrame() {
		for (int i = 0; i < mainMemory.length; ++i) {
			if (mainMemory[i] == null) {
				return i;
			}
		}
		return NO_FREE;
	}
	
	/*
	 * ========================================================================================================
	 * Frame Reference Stack methods
	 * ========================================================================================================
	 */
	private void updateFrameReferenceStack(int frame) {
		/*
		 * If the frame was already in our reference stack we need to remove it
		 * and re-add at the top of the stack. If it wasn't there in the first place
		 * then the remove does nothing
		 */
		frameRefStack.remove(new Integer(frame));
		frameRefStack.add(frame);
	}
	
	private int getVictim() {
		// Index remove
		return frameRefStack.remove(STACK_BOTTOM);
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

		@Override
		public int hashCode() {
			final int prime = 31;
			int result = 1;
			result = prime * result + getOuterType().hashCode();
			result = prime * result
					+ ((owningThread == null) ? 0 : owningThread.hashCode());
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
			Frame other = (Frame) obj;
			if (!getOuterType().equals(other.getOuterType()))
				return false;
			if (owningThread == null) {
				if (other.owningThread != null)
					return false;
			} else if (!owningThread.equals(other.owningThread))
				return false;
			return true;
		}

		private MemoryManagementUnit getOuterType() {
			return MemoryManagementUnit.this;
		}
		

	}
}
