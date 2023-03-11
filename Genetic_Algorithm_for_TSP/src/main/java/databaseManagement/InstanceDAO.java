/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package databaseManagement;

import java.util.List;

/**
 *
 * @author Tudor Onofrei
 */
public interface InstanceDAO {
    public InstanceEntity getInstanceById(int id);
    public InstanceEntity getInstanceByName(String numeInstanta);
    public List<InstanceEntity> getAllInstances();
    
    public void insertInstance(String nume, String caleFisier, int best);
    
    public void updateInstanceName(InstanceEntity instantaDeUpdatat, String nume);
    public void updateInstanceBest(InstanceEntity instantaDeUpdatat, int newBest);
    public void updateInstanceId(InstanceEntity instantaDeUpdatat, int newId);
    
    public void deleteInstanceById(int idInstanta);
    public void deleteInstanceByName(String name);

    public InstanceEntity findById(int idInstanta);
    public InstanceEntity findByName(String numeInstanta);
} 
