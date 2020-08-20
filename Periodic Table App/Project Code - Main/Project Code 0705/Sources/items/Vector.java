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
      double finalX = getElement(1, 1) + x;
      double finalY = getElement(1, 2) + y;
      
      setElement(1, 1, finalX);
      setElement(1, 2, finalY);
   }
   
   /**
    * Adds given vector to existing vector
    * 
    * @param otherVector Other vector to add to existing vector
    */
   public void add(Vector otherVector)
   {
      double finalX = getElement(1, 1) + otherVector.getElement(1, 1);
      double finalY = getElement(1, 2) + otherVector.getElement(1, 2);
      
      setElement(1, 1, finalX);
      setElement(1, 2, finalY);
   }
   
}