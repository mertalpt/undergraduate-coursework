package field;

/**
 * Class representing a gravitational field
 * 
 * @author Mert Alp TAYTAK
 * @version 07/05/2017
 */

// IMPORTS

import items.*;
import java.util.ArrayList;

// CLASS

public class GravitationalField extends Field
{
   // FIELDS
   
   private final static double GRAVITATIONAL_CONSTANT = 6.67384e-11;
   
   // PROPERTIES
   
   
   // CONSTRUCTORS
   
   public GravitationalField(int width, int heigth, double sensitivity, ArrayList<Particle> particles)
   {
      super(width, heigth, sensitivity, particles);      
   }
   
   // METHODS
      
   /**
    * Calculates field strength based on listed particles and updates the field 
    */
   public void update()
   {
      super.resetField();
      double strengthCoeff;
      
      for (Particle particle : super.getParticles() )
      {
         strengthCoeff = GRAVITATIONAL_CONSTANT * particle.getMass();
         super.generateField(strengthCoeff, particle.getXPos(), particle.getYPos() );
      }
   }
   
   
}