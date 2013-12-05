package edu.iastate.cs352.project2.main;

import edu.iastate.cs352.project2.logic.MemoryManagementUnit;
import edu.iastate.cs352.project2.logic.UserProcess;

/**
 * Main class used to fire up the simulation
 * @author Brandon
 *
 */
public class simVm {

	private static int size, numPages, numFrames, numUserProc;
	private static final boolean DEBUGGING = true;
	
	public static void main(String args[]) {
		if (args.length != 4) {
			System.out.println("ERROR: Not enough program arguments. Should be 4 but was: " + args.length);
			System.exit(0);
		}
		
		size = numPages = numFrames = numUserProc = 0;
		
		try {
			size = Integer.parseInt(args[0]);
			numPages = Integer.parseInt(args[1]);
			numFrames = Integer.parseInt(args[2]);
			numUserProc = Integer.parseInt(args[3]);
		} catch (NumberFormatException nfe) {
			System.out.println("ERROR: Program arguments are not all integers!");
			System.exit(0);
		}
		
		if (DEBUGGING) {
			overwriteValues();
		}
		
		MemoryManagementUnit mmu = new MemoryManagementUnit(numFrames);
		
		// FIRE 'EM UP!
		for (int i = 0; i < numUserProc; ++i) {
			final UserProcess proc = new UserProcess(i, size, numPages, mmu);
			new Thread(new Runnable() {
				@Override
				public void run() {
					proc.run();
				}
			}).start();
		}
		
	}

	private static void overwriteValues() {
		size = 8;
		numPages = 3;
		numFrames = 4;
		numUserProc	= 2;
	}
}
