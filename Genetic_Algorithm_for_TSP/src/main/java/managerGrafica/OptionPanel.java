/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package managerGrafica;

import javax.swing.BoxLayout;
import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSpinner;
import javax.swing.SpinnerNumberModel;
/**
 *
 * @author Tudor Onofrei
 */
public class OptionPanel extends JPanel{
    final MainFrame frame;
    JComboBox shapeCombo; 
    JLabel mesajOptiuni; 
    JCheckBox deleteCheck;
    JCheckBox freeCheck;
    
    JLabel numarGeneratiiLabel;
    JLabel probabilitateMutatieLabel;
    JLabel probabilitateCrossLabel;
    JLabel numarIndiviziPopulatieLabel;
    
    JSpinner numarGeneratiiSpinner;
    public JSpinner probabilitateMutatieSpinner;
    public JSpinner probabilitateCrossSpinner;
    JSpinner numarIndiviziPopulatieSpinner;
    
    public OptionPanel(MainFrame frame) {
        this.frame = frame;
        init();
    }

    private void init() {
        
        setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));
        
        numarGeneratiiLabel = new JLabel("Numar generatii: ");
        numarGeneratiiSpinner = new JSpinner(new SpinnerNumberModel(100, 100, 2000, 1));
        
        probabilitateMutatieLabel = new JLabel("Probabilitate Mutatie: ");
        probabilitateMutatieSpinner = new JSpinner(new SpinnerNumberModel(0.1, 0, 1, 0.01));
        
        probabilitateCrossLabel = new JLabel("Probabilitate Cross: ");
        probabilitateCrossSpinner = new JSpinner(new SpinnerNumberModel(0.3, 0, 1, 0.01));
        
        numarIndiviziPopulatieLabel= new JLabel("Numar indivizi populatie: ");
        numarIndiviziPopulatieSpinner = new JSpinner(new SpinnerNumberModel(100, 100, 200, 1));
        
       add(numarIndiviziPopulatieLabel);
       add(numarIndiviziPopulatieSpinner);
       add(numarGeneratiiLabel);
       add(numarGeneratiiSpinner);
       add(probabilitateMutatieLabel);
       add(probabilitateMutatieSpinner);
       add(probabilitateCrossLabel);
       add(probabilitateCrossSpinner);
    }
}
