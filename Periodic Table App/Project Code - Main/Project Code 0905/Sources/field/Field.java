package field;

/**
 * Class blueprint for fields
 * 
 * @author Mert Alp TAYTAK
 * @version 07/05/2017
 */

// IMPORTS

import java.util.ArrayList;
import items.*;

// CLASS

public abstract class Field
{
   // FIELDS
     
   
   // PROPERTIES
   
   // Field distrubition, spacing and size wise
   private int width;
   private int heigth;   
   private double sensitivity;
   
   // Field strength at points
   private Vector[][] strength;
   
   // Field members
   private ArrayList<Particle> particles;
   
   // CONSTRUCTORS
   
   /**
    * Default constructor for a field
    * 
    * @param width Width of the field in coordinates
    * @param heigth Heigth of the field in coordinates
    * @param sensitivity Space between each coordinate in meters
    */
   public Field(int width, int heigth, double sensitivity, ArrayList<Particle> particles)
   {
      // Initializing distrubition properties
      this.width = width;
      this.heigth = heigth;
      this.sensitivity = sensitivity;
      
      // Initializing field    
      strength = new Vector[width][heigth];
      this.particles = particles;
      resetField();
   }
   
   // METHODS
   
   /**
    * String representation of the field
    * 
    * @return A string representing the field
    */
   public String toString()
   {
      String returnString = "";
      
      for (int i = 0; i < heigth; i++)
      {
         for (int j = 0; j < width; j++)
         {
            returnString += getStrength(j, i).toString();
            if (j != width - 1)
               returnString += "; ";
         }
         returnString += "\n";
      }
      
      return returnString;
   }
   
   /**
    * Returns field strength at given point
    * 
    * @param x X coordinate of the field
    * @param y Y coordinate of the field
    */
   public Vector getStrength(double x, double y)
   {
      double cordX = x;
      double cordY = y;
      
      if (Double.isNaN(cordX) )
         cordX = 0;
      if (Double.isNaN(cordY) )
         cordY = 0;
      
      if (cordX < 0 || cordX >= width || cordY < 0 || cordY >= heigth)
      {
         System.out.println("Error! Particle out of field.");
         return new Vector();
      }
      
      return strength[(int) cordX][(int) cordY];
   }
   
   /**
    * Resets every point of the field to 0
    */
   public void resetField()
   {
      for (int i = 0; i < strength.length; i++)
      {
         for (int j = 0; j < strength[0].length; j++)
         {
            strength[i][j] = new Vector();
         }
      }
   }
   
  /**
   * Sets arrayList for particles
   * 
   * @param particles Given list of particles
   */
   public void setParticles(ArrayList<Particle> particles)
   {
      this.particles = particles;
   }
   
   /**
    * Allows access to particles list from sub classes
    * 
    * @return Particles array list
    */
   protected ArrayList<Particle> getParticles()
   {
      return particles;
   }
   
   /**
    * Generates field based on a starting point and a field strength coefficient
    * 
    * @param strengthCoefficient Modifier for field strength that will be calculated different for each different field
    * @param x Origin X
    * @param y Origin Y
    */
   protected void generateField(double strengthCoefficient, double x, double y)
   {
      final double X_ORIGIN = x;
      final double Y_ORIGIN = y;
      final double STR_COEFF = strengthCoefficient;
      
      double fieldStrength;
      double distance;
      double x_pointer;
      double y_pointer;
      double x_value;
      double y_value;
      
      for (int i = 0; i < width; i++)
      {
         for (int j = 0; j < heigth; j++)
         {
            distance = Math.sqrt( Math.pow(x - i, 2) + Math.pow(y - j, 2) ) * sensitivity;
                                 
            fieldStrength = STR_COEFF / Math.pow(distance, 2);
            
            x_pointer = sensitivity * (x - i) / distance;
            y_pointer = sensitivity * (y - j) / distance;
            
            x_value = fieldStrength * x_pointer;
            y_value = fieldStrength * y_pointer;
            
            if (Double.isNaN(x_value) )
               x_value = 0;
            
            if (Double.isNaN(y_value) )
               y_value = 0;
            
            strength[i][j].add( x_value, y_value );
         }
      }
   }
   
//   /** OLD VERSION
//    * Generates field based on a starting point and a field strength coefficient
//    * 
//    * @param strengthCoefficient Modifier for field strength that will be calculated different for each different field
//    * @param x Origin X
//    * @param y Origin Y
//    */
//   protected void generateField(double strengthCoefficient, double x, double y)
//   {
//      final double X_ORIGIN = x;
//      final double Y_ORIGIN = y;
//      final double STR_COEFF = strengthCoefficient;
//      
//      double x_pointer;
//      double y_pointer;
//      double x_value;
//      double y_value;
//      
//      for (int i = 0; i < width; i++)
//      {
//         for (int j = 0; j < heigth; j++)
//         {
//            x_pointer = x - i;
//            y_pointer = y - j;
//            
//            x_value = (x_pointer * STR_COEFF) / Math.pow(x_pointer, 2);
//            y_value = (y_pointer * STR_COEFF) / Math.pow(y_pointer, 2);
//            
//            if (Double.isNaN(x_value) )
//               x_value = 0;
//            
//            if (Double.isNaN(y_value) )
//               y_value = 0;
//            
//            strength[i][j].add( x_value, y_value );
//         }
//      }
//   }
   
}