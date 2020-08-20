/**
 * Electrical field test class
 * 
 * @author Mert Alp TAYTAK
 * @version 09/05/2017
 */

// IMPORTS

import field.*;
import items.*;
import java.util.ArrayList;

// CLASS

public class TestElectricalField
{
   public static void main(String[] args)
   {
      // CONSTANTS
      
      
      // VARIABLES
      
      
     // INSTANCES
      
      ArrayList<Particle> particles;
      ElectricalField field;
      Particle particle1;
      Particle particle2;
      
      // MAIN CODE
      
      particles = new ArrayList<Particle>();
      field = new ElectricalField(4, 4, Math.pow(10, -9), particles);
      
      System.out.println("Field toString: \n" + field.toString() );
      
      particle1 = new Particle(8, 10, 10);
      particle1.setPosition(0, 0);
      particles.add(particle1);
      
      System.out.println("Particle1 toString: \n" + particle1.toString() );
      
      field.update();
      
      System.out.println("Field toString: \n" + field.toString() );
      
      particle2 = new Particle(4, 3, 2);
      particle2.setPosition(2, 3);
      particles.add(particle2);
      
      System.out.println("Particle2 toString: \n" + particle2.toString() );
      
      field.update();
      
      System.out.println("Field toString: \n" + field.toString() );
   }
}