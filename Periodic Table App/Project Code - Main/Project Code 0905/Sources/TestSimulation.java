/**
 * Test class for SimulationModel
 * 
 * @author Mert Alp TAYTAK
 * @version 09/05/2017
 */

// IMPORTS

import items.*;
import field.*;

// CLASS

public class TestSimulation
{
   public static void main(String[] args)
   {
      // CONSTANTS
      
      
      // VARIABLES
      
      
      // INSTANCES
      
      SimulationModel simulation;
      Particle particle1;
      Particle particle2;
      Particle particle3;
      Particle particle4;
      
      // MAIN CODE
      
      simulation = new SimulationModel(10, 10, Math.pow(10, -6) );
      
      particle1 = new Particle(8, 8, 10);
      particle2 = new Particle(11, 12, 10);
      particle3 = new Particle(9, 8, 10);
      particle4 = new Particle(12, 12, 10);
      
      particle1.setPosition(0, 2);
      particle2.setPosition(6, 9);
      particle3.setPosition(5, 5);
      particle4.setPosition(6, 4);
      
      simulation.addParticle(particle1);
      simulation.addParticle(particle2);
      simulation.addParticle(particle3);
      simulation.addParticle(particle4);
      
      for (int i = 0; i < 5; i++)
      {
         simulation.update();      
         System.out.println("Simulation toString: \n" + simulation.toString() );
      }
   }
}