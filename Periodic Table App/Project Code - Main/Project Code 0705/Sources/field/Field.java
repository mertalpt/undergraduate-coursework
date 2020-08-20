package field;

/**
 * Class blueprint for fields
 * 
 * @author Mert Alp TAYTAK
 * @version 07/05/2017
 * 
 *  TO DO:
 * - Write all update methods
 * - Test
 */

// IMPORTS

import java.util.ArrayList;
import items.*;

// CLASS

public abstract class Field
{
   // FIELDS
   
   private final int WIDTH_COUNT;
   private final int HEIGTH_COUNT;
   
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
    * @param width Width of the field
    * @param heigth Heigth of the field
    * @param sensitivity How well the field is approximated
    */
   public Field(int width, int heigth, double sensitivity)
   {
      // Initializing distrubition properties
      this.width = width;
      this.heigth = heigth;
      this.sensitivity = sensitivity;
      
      // Initializing field
      WIDTH_COUNT = (int) ( (double) width / sensitivity);
      HEIGTH_COUNT = (int) ( (double) width / sensitivity);
      strength = new Vector[WIDTH_COUNT][HEIGTH_COUNT];
      particles = new ArrayList<Particle>();
   }
   
   // METHODS
   
   /**
    * Returns field strength at given point
    * 
    * @param x X coordinate of the field
    * @param y Y coordinate of the field
    */
   public Vector getStrength(int x, int y)
   {
      return strength[x][y];
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
    * Adds a particle to the field
    * 
    * @param particle Particle to add
    */
   public void addParticle(Particle particle)
   {
      particles.add(particle);
   }
   
   /**
    * Removes particle from the field
    * 
    * @param particle Particle to remove
    */
   public void removeParticle(Particle particle)
   {
      // To do
   }
   
   /**
    * Generates field based on a starting point and a field strength coefficient
    * 
    * @param strengthCoefficient Modifier for field strength that will be calculated different for each different field
    * @param x Origin X
    * @param y Origin Y
    */
   public void generateField(double strengthCoefficient, double x, double y)
   {
      final double X_ORIGIN = x;
      final double Y_ORIGIN = y;
      final double STR_COEFF = strengthCoefficient;
      
      double x_pointer;
      double y_pointer;
      
      for (int i = 0; i < WIDTH_COUNT; i++)
      {
         for (int j = 0; j < HEIGTH_COUNT; j++)
         {
            x_pointer = x - i;
            y_pointer = y - i;
            
            strength[i][j].add( (x_pointer * STR_COEFF) / Math.pow(x_pointer, 2), (y_pointer * STR_COEFF) / Math.pow(y_pointer, 2) );
         }
      }
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
}