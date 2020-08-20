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
   private static final double BOHR_RADIUS = 5.2918e-11; // Bohr radius in meters
   
   // PROPERTIES
   
   private int index;
   
   // Basic sub-atomic particles
   private int protonCount;
   private int neutronCount;
   private int electronCount;
   
   // Characteristics
   private int atomicCharge;
   private double electricCharge;
   private double mass;
   private double radius;
   
   // Position-Velocity-Acceleration information
   private double x_pos;
   private double y_pos;
   private double x_vel;
   private double y_vel;
   private double x_acc;
   private double y_acc;
   
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
    * String representation of the Particle
    * 
    * @return A string representing this particle
    */
   public String toString()
   {
      String returnString = "";
      
      // Basic sub-atomic particle info
      returnString = returnString + "p: " + protonCount + ", n: " + neutronCount + ", e:" + electronCount + "\n";
      
      // Characteristics info
      returnString = returnString + "AtomCh: " + atomicCharge + ", ElecCh: " + electricCharge + ", Mass: " + mass + ", Radius: " + radius + "\n";
      
      // Pos-Vel-Acc info
      returnString = returnString + "xPos: " + x_pos + ", yPos: " + y_pos + ", xVel: " + x_vel + ", yVel: " + y_vel + ", xAcc: " + x_acc + ", yAcc: " + y_acc;
      
      return returnString;
   }
   
   /**
    * Updates everything about the particle
    */
   public void update(Vector electricFieldStrength)
   {
      Vector ELECTRIC_FIELD_STRENGTH = electricFieldStrength;
      
      // Position change due acceleration
      x_pos = x_pos + x_vel;
      y_pos = y_pos + y_vel;
                  
      // Velocity change due acceleration
      x_vel = x_vel + x_acc;
      y_vel = y_vel + y_acc;
      
      // Acceleration due electrical forces
      x_acc = x_acc + (getElectricCharge() * ELECTRIC_FIELD_STRENGTH.getXComponent() / getMass() );
      y_acc = y_acc + (getElectricCharge() * ELECTRIC_FIELD_STRENGTH.getYComponent() / getMass() );
   }
   
   /**
    * Updates particle characteristics
    */
   private void updateCharacteristics()
   {
      atomicCharge = protonCount - electronCount;
      electricCharge = atomicCharge * CHARGE_CONSTANT;
      mass = P_MASS_CONSTANT * protonCount + N_MASS_CONSTANT * neutronCount + E_MASS_CONSTANT * electronCount;
      radius = (BOHR_RADIUS / protonCount) * ( (electronCount / 8) + 1); // Approximation
   }
   
   // Getters
   
   public int getIndex()
   {
      return index;
   }
   
   public int getAtomicCharge()
   {
      return atomicCharge;
   }
   
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
   
   public void setIndex(int index)
   {
      this.index = index;
   }
   
   public void setPosition(double x, double y)
   {
      x_pos = x;
      y_pos = y;
   }
}