/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package managerGrafica;

import databaseManagement.InstanceManipulator;
import geneticAlgorithm.GeneticAlgoritm;
import java.awt.BorderLayout;
import javax.swing.*;

/**
 *
 * @author Tudor Onofrei
 */
public class MainFrame extends JFrame {

    ConfigPanel configPanel;
    ControlPanel controlPanel;
    public DrawingPanel canvas;
    public OptionPanel optionPanel;
    public ContainerForme containerForms;
    InstanceManipulator instante = new InstanceManipulator();
    public GeneticAlgoritm algoritmGenetic;
    
    public MainFrame() {
        super("MyGA Visualiser");
        init();
    }

    private void init() {
        setDefaultCloseOperation(EXIT_ON_CLOSE);

        canvas = new DrawingPanel(this);
        controlPanel = new ControlPanel(this);
        configPanel = new ConfigPanel(this);
        optionPanel = new OptionPanel(this);
        containerForms = new ContainerForme(this);
        
        add(canvas, BorderLayout.CENTER); 
        add(configPanel, BorderLayout.NORTH);
        add(optionPanel, BorderLayout.WEST);
        add(controlPanel, BorderLayout.SOUTH);
        
        pack();
    }
}
