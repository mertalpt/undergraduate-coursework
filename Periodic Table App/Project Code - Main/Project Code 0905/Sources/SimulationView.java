/**
 * Frame for simulation view
 * 
 * @author Mert Alp TAYTAK
 * @version 10/05/2017
 */

// IMPORTS

import javax.swing.*;
import java.awt.*;
import field.*;
import items.*;
import java.util.ArrayList;

// CLASS

public class SimulationView extends JFrame
{
   // FIELDS
   
   
   // PROPERTIES
   
   SimulationPanel panel;
   
   // CONSTRUCTORS
   
   public SimulationView()
   {
      panel = new SimulationPanel();
      panel.setPreferredSize(new Dimension(900, 900) );
      setSize(1000, 1000);
      setTitle("Simulation");
      setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
      
      add(panel);
      setVisible(true);
   }
   
   // METHODS
   
   public void update()
   {
      panel.update();
   }
   
   // SUBCLASSES
   
   private class SimulationPanel extends JPanel
   {      
      // FIELDS
      
      final int WIDTH = 900;
      final int HEIGTH = 900;
      final double SENSITIVITY = Math.pow(10, -1);
      final int PIXEL_SPACING = 1;
      
      // PROPERTIES
      
      SimulationModel simulation;
      ElectricalField field;
      ArrayList<Particle> particles;
      
      // CONSTRUCTORS
      
      public SimulationPanel()
      {
         simulation = new SimulationModel(WIDTH, HEIGTH, SENSITIVITY);
         
         Particle particle1 = new Particle(3, 2, 2);
         Particle particle2 = new Particle(4, 2, 2);
         Particle particle3 = new Particle(8, 8, 10);
         Particle particle4 = new Particle(7, 7, 10);
         Particle particle5 = new Particle(19, 20, 20);
         particle1.setPosition(400, 800);
         particle2.setPosition(300, 200);
         particle3.setPosition(700, 500);
         particle4.setPosition(100, 100);
         particle5.setPosition(600, 400);
         
         simulation.addParticle(particle1);
         simulation.addParticle(particle2);
         simulation.addParticle(particle3);
         simulation.addParticle(particle4);
         simulation.addParticle(particle5);
         
         simulation.update();
         
         field = simulation.getElectricalField();
         particles = simulation.getParticles();
      }
      
      // METHODS
      
      public void paint(Graphics g)
      {
         super.paint(g);
         
         // Draw field vectors
         
         Vector tempVector;
         
         for (int i = 0; i < WIDTH; i++)
         {
            for (int j = 0; j < HEIGTH; j++)
            {
               tempVector = field.getStrength(i, j);
               
               g.setColor(Color.ORANGE);
               //g.fillOval(i * PIXEL_SPACING, j * PIXEL_SPACING, 5, 5);
               
               g.setColor(Color.BLACK);
               //g.drawLine(i * PIXEL_SPACING, j * PIXEL_SPACING, (int) (tempVector.getXComponent() * PIXEL_SPACING * SENSITIVITY), (int) (tempVector.getYComponent() * PIXEL_SPACING * SENSITIVITY) );
            }
         }
         
         // Draw particles
         
         for (Particle particle : particles)
         {
            if (particle.getElectricCharge() == 0)
               g.setColor(Color.BLUE);
            else if(particle.getElectricCharge() < 0)
               g.setColor(new Color( 255 - ( ( Math.abs(particle.getAtomicCharge() * 50) < 256) ? -1 * (particle.getAtomicCharge() * 50) : 0), 0, 0) );
            else
               g.setColor(new Color( 0, 255 - ( (particle.getAtomicCharge() * 50 < 256) ? particle.getAtomicCharge() * 50 : 0), 0) );
            g.fillOval( (int) particle.getXPos() * PIXEL_SPACING, (int) particle.getYPos() * PIXEL_SPACING, 20, 20);
         }
      }
      
      public void update()
      {
         simulation.update();
         //System.out.println(simulation.toString() );
         repaint();
      }
      
   }
}