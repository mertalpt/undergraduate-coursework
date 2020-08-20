/**
 * Gravitational field test class
 * 
 * @author Mert Alp TAYTAK
 * @version 09/05/2017
 */

// IMPORTS

import field.*;
import items.*;
import java.util.ArrayList;

// CLASS

public class TestGravitationalField
{
   public static void main(String[] args)
   {
      // CONSTANTS
      
      
      // VARIABLES
      
      
      // INSTANCES
      
      ArrayList<Particle> particles;
      GravitationalField field;
      Particle particle1;
      
      // MAIN CODE
      
      particles = new ArrayList<Particle>();
      field = new GravitationalField(10, 10, 0.0000001, particles);
      
      System.out.println("Field toString: \n" + field.toString() );
      
      particle1 = new Particle(10000, 10000, 10000);
      particle1.setPosition(4, 3);
      
      System.out.println("Particle toString: \n" + particle1.toString() );
      
      field.update();
      
      System.out.println("Field toString: \n" + field.toString() );
   }
}