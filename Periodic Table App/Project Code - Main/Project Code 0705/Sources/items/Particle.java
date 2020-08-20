package items;

/**
 * Class defining a particle
 * 
 * @author Mert Alp TAYTAK
 * @version 01/05/2017
 */

// IMPORTS


// CLASS

public class Particle
{
   // FIELDS
   
   private static final double P_MASS_CONSTANT = 1.6726219e-27; // Sub-atomic mass in Kilograms
   private static final double N_MASS_CONSTANT = 1.674927471e-27; // Sub-atomic mass in Kilograms
   private static final double E_MASS_CONSTANT = 9.10938356e-31; // Sub-atomic mass in Kilograms
   private static final double CHARGE_CONSTANT = 1.60217646e-19; // Sub-atomic charge in Coulombs
   
   // PROPERTIES
   
   // Basic sub-atomic particles
   private int protonCount;
   private int neutronCount;
   private int electronCount;
   
   // Characteristics
   private int atomicCharge;
   private double electricCharge;
   private double mass;
   
   // Position-Velocity information
   private double x_pos;
   private double y_pos;
   private double x_vel;
   private double y_vel;
   
   // CONSTRUCTORS   
   
   /**
    * Basic particle constructor
    * 
    * @param protonCount Number of protons in the particle
    * @param neutronCount Number of neutrons in the particle
    * @param electronCount Number of electrons in the particle
    */
   public Particle(int protonCount, int neutronCount, int electronCount)
   {
      this.protonCount = protonCount;
      this.neutronCount = neutronCount;
      this.electronCount = electronCount;
      
      updateCharacteristics();
   }
   
   // METHODS
   
   /**
    * Updates everything about the particle
    */
   public void update()
   {
      // To do
   }
   
   /**
    * Updates particle characteristics
    */
   private void updateCharacteristics()
   {
      atomicCharge = protonCount - electronCount;
      electricCharge = atomicCharge * CHARGE_CONSTANT;
      mass = P_MASS_CONSTANT * protonCount + N_MASS_CONSTANT * neutronCount + E_MASS_CONSTANT * electronCount;
   }
   
   // Getters
   
   public double getElectricCharge()
   {
      return electricCharge;
   }
   
   public double getMass()
   {
      return mass;
   }
   
   public double getXPos()
   {
      return x_pos;
   }
   
   public double getYPos()
   {
      return y_pos;
   }
   
   public double getXVel()
   {
      return x_vel;
   }
   
   public double getYVel()
   {
      return y_vel;
   }
   
   // Setters
   
   
}