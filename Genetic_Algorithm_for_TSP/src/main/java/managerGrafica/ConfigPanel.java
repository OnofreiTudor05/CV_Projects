/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package managerGrafica;

import databaseManagement.InstanceEntity;
import geneticAlgorithm.GeneticAlgoritm;
import geneticAlgorithm.ProblemInstance;
import java.awt.event.ActionEvent;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;

/**
 *
 * @author Tudor Onofrei
 */
public class ConfigPanel extends JPanel {

    final MainFrame frame;
    
    JButton butonStart;
    JComboBox numeInstanteCombo;
    JLabel numeInstanteLabel;
    
    public ConfigPanel(MainFrame frame) {
        this.frame = frame;
        init();
    }

    private void init() {
        
        numeInstanteLabel = new JLabel("Alegeti o instanta: ");
        String[] instanceValues = new String[this.frame.instante.returnAll().size()];
        int indice = 0;
        for(InstanceEntity instantaInBD : this.frame.instante.returnAll()){
            instanceValues[indice] = new String(instantaInBD.getNume());
            indice++;
        }
        numeInstanteCombo = new JComboBox(instanceValues);
        
        butonStart = new JButton("Start");
        butonStart.addActionListener(this::start);
        
        
        add(numeInstanteLabel);
        add(numeInstanteCombo);
        add(butonStart);
        
    }
    
    public void start(ActionEvent e){
        frame.algoritmGenetic = new GeneticAlgoritm(new ProblemInstance(frame.instante.findByName(frame.configPanel.numeInstanteCombo.getSelectedItem().toString())));
        
        
        frame.algoritmGenetic.setNumarGeneratii(Integer.parseInt(frame.optionPanel.numarGeneratiiSpinner.getValue().toString()));
        frame.algoritmGenetic.setDimensiunePopulatie(Integer.parseInt(frame.optionPanel.numarIndiviziPopulatieSpinner.getValue().toString()));
        frame.algoritmGenetic.setProbabilitateCrossOver(Float.parseFloat(frame.optionPanel.probabilitateCrossSpinner.getValue().toString()));
        frame.algoritmGenetic.setProbabilitateMutatie(Float.parseFloat(frame.optionPanel.probabilitateMutatieSpinner.getValue().toString()));
        
        frame.algoritmGenetic.selfSolve(this.frame);
    
        
    }
    
}
