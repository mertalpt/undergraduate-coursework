package items;

/**
 * Class representing a Vector
 * 
 * @author Mert Alp TAYTAK
 * @version 07/05/2017
 */

// IMPORTS

import matrices.Matrix;

// CLASSES

public class Vector extends Matrix
{
   // FIELDS
   
   
   // PROPERTIES
   
   
   // CONSTRUCTORS
   
   /**
    * Constructs a matrix of dimension 1x2
    */
   public Vector()
   {
      super(1, 2);
   }
   
   // METHODS
   
   /**
    * Adds given <x, y> to existing vector
    * 
    * @param x X value to add
    * @param y Y value to add
    */
   public void add(double x, double y)
   {
      double finalX = getElement(0, 0) + x;
      double finalY = getElement(0, 1) + y;
      
      setElement(0, 0, finalX);
      setElement(0, 1, finalY);
   }
   
   /**
    * Adds given vector to existing vector
    * 
    * @param otherVector Other vector to add to existing vector
    */
   public void add(Vector otherVector)
   {
      double finalX = getElement(0, 0) + otherVector.getElement(0, 0);
      double finalY = getElement(0, 1) + otherVector.getElement(0, 1);
      
      setElement(0, 0, finalX);
      setElement(0, 1, finalY);
   }
   
   public double getXComponent()
   {
      return getElement(0, 0);
   }
   
   public double getYComponent()
   {
      return getElement(0, 1);
   }
   
}