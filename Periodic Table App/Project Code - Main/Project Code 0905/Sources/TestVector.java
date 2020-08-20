/**
 * Vector test class
 * 
 * @author Mert Alp TAYTAK
 * @version 09/05/2017
 */

// IMPORTS

import items.*;

// CLASS

public class TestVector
{
   public static void main(String[] args)
   {
      // CONSTANTS
      
      
      // VARIABLES
      
      
      // INSTANCES
      
      Vector vector1;
      Vector vector2;
      
      // MAIN CODE
      
      vector1 = new Vector();
      
      System.out.println("Vector1 toString: \n" + vector1.toString() );
      
      vector1.add(2, 3);
      
      System.out.println("Vector1 toString: \n" + vector1.toString() );
      
      vector2 = new Vector();
      vector2.add(1, 1);
      
      System.out.println("Vector2 toString: \n" + vector2.toString() );
      
      vector1.add(vector2);
      
      System.out.println("Vector1 toString: \n" + vector1.toString() );
      
   }
}