/**
 * Modeler class for simulation
 * 
 * @author Mert Alp TAYTAK
 * @version 09/05/2017
 */

// IMPORTS

import field.*;
import items.*;
import java.util.ArrayList;

// CLASS

public class SimulationModel
{
   // FIELDS
   
   private final int SIM_WIDTH;
   private final int SIM_HEIGTH;
   private final double SIM_SENSITIVITY;
   
   private int particleCount;
   
   // PROPERTIES
   
   ElectricalField electricField;
   ArrayList<Particle> particles;
   
   // CONSTRUCTORS
   
   /**
    * Constructor for the simulation model
    * 
    * @param width Width of the simulation
    * @param heigth Heigth of the simulation
    * @param sensitivity Space between coordinates in meters
    */
   public SimulationModel(int width, int heigth, double sensitivity)
   {
      SIM_WIDTH = width;
      SIM_HEIGTH = heigth;
      SIM_SENSITIVITY = sensitivity;
      
      particleCount = 0;
      
      particles = new ArrayList<Particle>();
      electricField = new ElectricalField(SIM_WIDTH, SIM_HEIGTH, SIM_SENSITIVITY, particles);
   }
   
   // METHODS
   
   /**
    * String representation of the model
    * 
    * @return String representing the simulation model
    */
   public String toString()
   {
      String returnString = "";
      
      // Particles
      
      for (Particle particle : particles)
      {
         returnString += particle.toString() + "\n";
      }
      
      // Electric field
      
      returnString += "Electric field: \n" + electricField.toString();
      
      return returnString;
   }
   
   /**
    * Updates the model
    */
   public void update()
   {      
      for (Particle particle : particles)
      {
         // Get field coordinates
         double particleX = particle.getXPos();
         double particleY = particle.getYPos();
         
         Vector elecFieldStrength = electricField.getStrength(particleX, particleY);
         
         particle.update(elecFieldStrength);
         
         // Delete this later
         electricField.update();
      }
      
      electricField.update();
   }
   
   /**
    * Adds a particle to be tracked
    * 
    * @param particle Particle to add
    */
   public void addParticle(Particle particle)
   {
      particle.setIndex(particleCount);
      particleCount++;
      
      particles.add(particle);
   }
   
   /**
    * Removes particle at given index
    * 
    * @param index Index of the particle
    */
   public void removeParticle(int index)
   {
      particles.remove(index);
      particleCount--;
      
      for (Particle particle : particles)
      {
         if (particle.getIndex() > index)
            particle.setIndex(particle.getIndex() - 1);
      }
   }
   
   /**
    * Finds index of the particle at given coordinates
    * 
    * @param x X coordinate
    * @param y Y coordinate
    * @param coordinateSensitivity How far a particle can be from (x, y) before being selected
    * @return Index of the particle
    */
   public int indexOfParticleAt(int x, int y, double coordinateSensitivity)
   {
      for (Particle particle : particles)
      {
         if (Math.abs(particle.getXPos() - x) <= coordinateSensitivity && Math.abs(particle.getYPos() - y) <= coordinateSensitivity)
            return particle.getIndex();
      }
      
      // If nothing is found
      return -1;
   }
   
   public ElectricalField getElectricalField()
   {
      return electricField;
   }
   
   public ArrayList<Particle> getParticles()
   {
      return particles;
   }
}