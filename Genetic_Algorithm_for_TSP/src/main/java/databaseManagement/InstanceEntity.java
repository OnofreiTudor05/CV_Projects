/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package databaseManagement;

import java.sql.Clob;

/**
 *
 * @author Tudor Onofrei
 */
public class InstanceEntity {
    private int id;
    private String nume;
    private Clob instanta;
    private int best;
    
    public InstanceEntity(int id, String nume, Clob instanta, int best){
        this.id = id;
        this.nume = nume;
        this.instanta = instanta;
        this.best = best;
    }

    public String getNume() {
        return nume;
    }

    public void setNume(String nume) {
        this.nume = nume;
    }

    public Clob getInstanta() {
        return instanta;
    }

    public void setInstanta(Clob instanta) {
        this.instanta = instanta;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getBest() {
        return best;
    }

    public void setBest(int best) {
        this.best = best;
    }
   
}
