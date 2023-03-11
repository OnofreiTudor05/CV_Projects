/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package managerGrafica;

import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.WindowEvent;
import javax.swing.JButton;
import javax.swing.JPanel;

/**
 *
 * @author Tudor Onofrei
 */
public class ControlPanel extends JPanel {

    final MainFrame frame;
    JButton saveButton = new JButton("Save");
    JButton resetButton = new JButton("Reset");
    JButton exitButton = new JButton("Exit");
    JButton loadButton = new JButton("Load");
    
    public ControlPanel(MainFrame frame) {
        this.frame = frame;
        init();
    }

    private void init() {
        setLayout(new GridLayout(1, 1));
        
        exitButton.setVisible(true);
        
        exitButton.addActionListener(this::exit);
        
        add(exitButton);
       
    }
    
    private void exit(ActionEvent e){
        frame.instante.closeConnection();
        frame.dispatchEvent(new WindowEvent(frame, WindowEvent.WINDOW_CLOSING));
    }
    
    
}
