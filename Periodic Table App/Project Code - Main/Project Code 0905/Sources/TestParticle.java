/**
 * Particle test class
 * 
 * @author Mert Alp TAYTAK
 * @version 09/05/2017
 */

// IMPORTS

import items.*;

// CLASS

public class TestParticle
{
   public static void main(String[] args)
   {
      // CONSTANTS
      
      
      // VARIABLES
      
      
      // INSTANCES
      
      Particle particle1;
      Particle particle2;
      
      // MAIN CODE
      
      particle1 = new Particle(1, 0, 1);
      
      System.out.println("Particle1: \n" + particle1.toString() );
      
      particle2 = new Particle(3, 2, 2);
      
      System.out.println("Particle2: \n" + particle2.toString() );
   }
}