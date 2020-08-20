package field;

/**
 * Class representing an electrical field
 * 
 * @author Mert Alp TAYTAK
 * @version 07/05/2017
 */

// IMPORTS

import items.*;

// CLASS

public class ElectricalField extends Field
{
   // FIELDS
   
   private final static double ELECTRICAL_FIELD_CONSTANT = 8.987552e9;
   
   // PROPERTIES
   
   
   // CONTRUCTORS
   
   public ElectricalField(int width, int heigth, double sensitivity)
   {
      super(width, heigth, sensitivity);
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
         strengthCoeff = -1 * ELECTRICAL_FIELD_CONSTANT * particle.getElectricCharge();
         super.generateField(strengthCoeff, particle.getXPos(), particle.getYPos() );
      }
   }
}