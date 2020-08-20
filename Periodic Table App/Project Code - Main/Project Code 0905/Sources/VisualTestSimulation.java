/**
 * Visual test for simulation
 * 
 * @author Mert Alp TAYTAK
 * @version 10/05/2017
 */

// IMPORTS


// CLASS

public class VisualTestSimulation
{
   public static void main(String[] args)
   {
      // CONSTANTS
      
      
      // VARIABLES
      
      
      // INSTANCES
      
      SimulationView simView;
      
      // MAIN CODE
      
      simView = new SimulationView();
      
      for (int i = 0; i < 100; i++)
      {
         try {
            Thread.sleep(1000);
         } catch(InterruptedException ex) {
            Thread.currentThread().interrupt();
         }
         System.out.println("Tick: " + i);
         simView.update();
      }
   }
}