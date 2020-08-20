package matrices;

/**
 * Class representing a Matrix
 * 
 * @author Mert Alp TAYTAK
 * @version 18/02/2017
 */

// IMPORTS

import java.util.Arrays;

// CLASS

public class Matrix
{
   // PROPERTIES
   
   private double[][] matrix;
   private final int ROW_NUMBER;
   private final int COLUMN_NUMBER;
   
   // CONSTRUCTORS
   
   // Creates an empty matrix
   public Matrix(int rows, int columns)
   {
      matrix = new double[rows][columns];
      ROW_NUMBER = rows;
      COLUMN_NUMBER = columns;
   }
   
   // Transforms a two dimensional array into a matrix
   public Matrix(double[][] otherMatrix)
   {
      matrix = otherMatrix;
      ROW_NUMBER = matrix.length;
      COLUMN_NUMBER = matrix[0].length;
   }
   
   // Clones another matrix
   public Matrix(Matrix otherMatrix)
   {
      this(otherMatrix.matrix);
   }
   
   // METHODS
   
   // Get string representation of the matrix
   public String toString()
   {
      String returnString = "";
      
      for (int i = 0; i < ROW_NUMBER; i++)
      {
         returnString = returnString + Arrays.toString(matrix[i]);
         returnString += "\n";
      }
      
      return returnString;
   }
   
   // Get a specific element in the matrix
   public double getElement(int row, int column)
   {
      return matrix[row][column];
   }
   
   // Set a specific element in the matrix
   public void setElement(int row, int column, double value)
   {
      matrix[row][column] = value;
   }
   
   // Compares sizes of two matrices
   private boolean sizeEquals(Matrix otherMatrix)
   {
      return (this.ROW_NUMBER == otherMatrix.ROW_NUMBER && this.COLUMN_NUMBER == otherMatrix.COLUMN_NUMBER);
   }
   
   // Checks if two matrices can be multiplied
   private boolean canMultiply(Matrix otherMatrix)
   {
      return (this.COLUMN_NUMBER == otherMatrix.ROW_NUMBER);
   }
   
   // Adds two matrices together into a new matrix
   public Matrix add(Matrix otherMatrix)
   {
      if (!this.sizeEquals(otherMatrix) )
         return null;
      else
      {
         Matrix returnMatrix = new Matrix(this.ROW_NUMBER, this.COLUMN_NUMBER);
         
         for (int i = 0; i < this.ROW_NUMBER; i++)
         {
            for (int j = 0; j < this.COLUMN_NUMBER; j++)
            {
               returnMatrix.setElement(i,  j, (this.getElement(i, j) + otherMatrix.getElement(i, j) ) );
            }
         }
         
         return returnMatrix;
      }
   }
   
   // Multiplies two matrices and puts the result into a new matrix
   public Matrix multiply(Matrix otherMatrix)
   {
      if (!this.canMultiply(otherMatrix) )
         return null;
      else
      {
         Matrix returnMatrix = new Matrix(this.ROW_NUMBER, otherMatrix.COLUMN_NUMBER);
         
         for (int i = 0; i < this.ROW_NUMBER; i++)
         {
            for (int j = 0; j < otherMatrix.COLUMN_NUMBER; j++)
            {
               for (int k = 0; k < this.COLUMN_NUMBER; k++)
               {
                  returnMatrix.setElement(i,  j, (returnMatrix.getElement(i, j) + (this.getElement(i,  k) * otherMatrix.getElement(k,  j) ) ) );
               }
            }
         }
         
         return returnMatrix;
      }
   }
   
   // Transposes this matrix and returns it in a new matrix
   public Matrix transpose()
   {
      // To do
      return null;
   } 
}
