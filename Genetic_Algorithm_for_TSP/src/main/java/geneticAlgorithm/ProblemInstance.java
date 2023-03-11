/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package geneticAlgorithm;

import databaseManagement.InstanceEntity;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.io.Reader;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.sql.SQLException;
import java.util.Scanner;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Tudor Onofrei
 */
public class ProblemInstance {
    private int Cost[][];
    private String numeInstanta;
    private int numarNoduri;
    
    public ProblemInstance(InstanceEntity instantaAleasa){
        try {
            File fisierTemporal = new File("Instanta.txt");
            FileWriter output = new FileWriter(fisierTemporal);
            Reader input = instantaAleasa.getInstanta().getCharacterStream();
            int theChar;
                while ((theChar = input.read()) > 0) {
                    output.write(theChar);
                }
            
            input.close();
            output.flush();
            output.close();

            Scanner cititorFisier = new Scanner(fisierTemporal);
            numarNoduri = cititorFisier.nextInt();
            
            Cost = new int[numarNoduri+1][numarNoduri+1];
            
            for(int i = 0; i < numarNoduri; i++){
                for(int j = 0; j < numarNoduri; j++){
                    Cost[i][j] = cititorFisier.nextInt();
                    if(i == j){
                        Cost[i][j] = Integer.MAX_VALUE;
                    }
                }
            }
            cititorFisier.close();
            fisierTemporal.delete();
            
        } catch (IOException ex) {
            Logger.getLogger(ProblemInstance.class.getName()).log(Level.SEVERE, null, ex);
        } catch (SQLException ex) {
            Logger.getLogger(ProblemInstance.class.getName()).log(Level.SEVERE, null, ex);
        } 
    }
    
    public ProblemInstance(String caleFisier){
        try {
            Path pathFisier = Paths.get(caleFisier);
            
            numeInstanta = caleFisier.substring(caleFisier.lastIndexOf('\\') + 1);
            
            Scanner cititorFisier = new Scanner(pathFisier);
            numarNoduri = cititorFisier.nextInt();
            
            Cost = new int[numarNoduri+1][numarNoduri+1];
            
            for(int i = 0; i < numarNoduri; i++){
                for(int j = 0; j < numarNoduri; j++){
                    Cost[i][j] = cititorFisier.nextInt();
                    if(i == j){
                        Cost[i][j] = Integer.MAX_VALUE;
                    }
                }
            }
            System.out.println(this.toString());
            
        } catch (FileNotFoundException ex) {
            Logger.getLogger(ProblemInstance.class.getName()).log(Level.SEVERE, null, ex);
        } catch (IOException ex) {
            Logger.getLogger(ProblemInstance.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public float getCost(int nod, int vecin){
        return Cost[nod][vecin];
    }
    
    public int getDimensiune(){
        return numarNoduri;
    }
    
    @Override
    public String toString(){
        StringBuilder sirInstanta = new StringBuilder();
        
        sirInstanta.append("Nume instanta: ");
        sirInstanta.append(numeInstanta);
        sirInstanta.append("\n");
        sirInstanta.append(numarNoduri);
        sirInstanta.append("\n");
        for(int i = 0; i<numarNoduri; i++){
            for(int j = 0; j<numarNoduri; j++){
                sirInstanta.append(Cost[i][j]);
                sirInstanta.append(" ");
            }
            sirInstanta.append("\n");
        }
        
        return sirInstanta.toString();
    }

    public int getNumarNoduri() {
        return numarNoduri;
    }

    public void setNumarNoduri(int numarNoduri) {
        this.numarNoduri = numarNoduri;
    }
}
