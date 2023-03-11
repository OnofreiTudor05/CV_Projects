/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package databaseManagement;

import static databaseManagement.ConnectionManager.conexiuneDatabase;
import java.io.FileReader;
import java.io.IOException;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Tudor Onofrei
 */
public class InstanceManipulator implements InstanceDAO {

    private Connection instantaConexiune = ConnectionManager.getConnectionInstance();
    private List<InstanceEntity> instanteProbleme = new ArrayList<>();

    public InstanceManipulator() {
        this.getAllInstances();
    }

    @Override
    public InstanceEntity getInstanceById(int id) {
        InstanceEntity gasit = null;
        String query = "SELECT * FROM instance WHERE id = " + id;
        Statement state = null;
        ResultSet rs = null;
        try {
            state = instantaConexiune.createStatement();
            rs = state.executeQuery(query);

            if (rs.next()) {
                gasit = new InstanceEntity(rs.getInt("id"), rs.getString("nume"), rs.getClob("fisier"), rs.getInt("best"));
            }

        } catch (SQLException ex) {
            Logger.getLogger(InstanceManipulator.class.getName()).log(Level.SEVERE, null, ex);
        }
        return gasit;
    }

    @Override
    public InstanceEntity getInstanceByName(String numeInstanta) {
        InstanceEntity gasit = null;
        String query = "SELECT * FROM instance WHERE nume =" + " '" + numeInstanta + "';";
        Statement state = null;
        ResultSet rs = null;
        try {
            state = instantaConexiune.createStatement();
            rs = state.executeQuery(query);

            if (rs.next()) {
                gasit = new InstanceEntity(rs.getInt("id"), rs.getString("nume"), rs.getClob("fisier"), rs.getInt("best"));
            }

        } catch (SQLException ex) {
            Logger.getLogger(InstanceManipulator.class.getName()).log(Level.SEVERE, null, ex);
        }
        return gasit;
    }

    @Override
    public List<InstanceEntity> getAllInstances() {
        InstanceEntity gasit = null;
        String query = "SELECT * FROM instance;";
        Statement state = null;
        ResultSet rs = null;
        try {
            state = instantaConexiune.createStatement();
            rs = state.executeQuery(query);

            while (rs.next()) {
                gasit = new InstanceEntity(rs.getInt("id"), rs.getString("nume"), rs.getClob("fisier"), rs.getInt("best"));
                instanteProbleme.add(gasit);
            }

        } catch (SQLException ex) {
            Logger.getLogger(InstanceManipulator.class.getName()).log(Level.SEVERE, null, ex);
        }
        return instanteProbleme;
    }

    @Override
    public void updateInstanceName(InstanceEntity instantaDeUpdatat, String nume) {
        String query = "UPDATE instance SET nume =" + "'" + nume + "' WHERE nume = '" + instantaDeUpdatat.getNume() + "';";
        Statement state = null;
        try {
            state = instantaConexiune.createStatement();
            state.execute(query);
        } catch (SQLException ex) {
            Logger.getLogger(InstanceManipulator.class.getName()).log(Level.SEVERE, null, ex);
        } finally {
            System.out.println("Numele instantei a fost updatat cu succes!");
        }
    }

    @Override
    public void updateInstanceBest(InstanceEntity instantaDeUpdatat, int newBest) {
        String query = "UPDATE instance SET best =" + "'" + newBest + "' WHERE id = " + instantaDeUpdatat.getId() + ";";
        Statement state = null;
        try {
            state = instantaConexiune.createStatement();
            state.execute(query);
        } catch (SQLException ex) {
            Logger.getLogger(InstanceManipulator.class.getName()).log(Level.SEVERE, null, ex);
        } finally {
            System.out.println("Numele instantei a fost updatat cu succes!");
        }
    }

    @Override
    public void updateInstanceId(InstanceEntity instantaDeUpdatat, int newId) {
        String query = "UPDATE instance SET id =" + "'" + newId + "' WHERE id = " + instantaDeUpdatat.getId() + ";";
        Statement state = null;
        try {
            state = instantaConexiune.createStatement();
            state.execute(query);
        } catch (SQLException ex) {
            Logger.getLogger(InstanceManipulator.class.getName()).log(Level.SEVERE, null, ex);
        } finally {
            System.out.println("Numele instantei a fost updatat cu succes!");
        }

    }

    @Override
    public void deleteInstanceById(int idInstanta) {
        String query = "DELETE FROM instance WHERE id = " + idInstanta + ";";
        Statement state = null;
        try {
            state = instantaConexiune.createStatement();
            state.execute(query);
        } catch (SQLException ex) {
            Logger.getLogger(InstanceManipulator.class.getName()).log(Level.SEVERE, null, ex);
        } finally {
            System.out.println("Numele instantei a fost updatat cu succes!");
        }
    }

    @Override
    public void deleteInstanceByName(String name) {
        String query = "DELETE FROM instance WHERE name = '" + name + "';";
        Statement state = null;
        try {
            state = instantaConexiune.createStatement();
            state.execute(query);
        } catch (SQLException ex) {
            Logger.getLogger(InstanceManipulator.class.getName()).log(Level.SEVERE, null, ex);
        } finally {
            System.out.println("Numele instantei a fost updatat cu succes!");
        }
    }

    @Override
    public void insertInstance(String nume, String caleFisier, int best) {
        try {
            int idMaxim = 0;
            String maxId = "SELECT * FROM `instance` ORDER BY id DESC LIMIT 1";
            Statement statement1 = instantaConexiune.createStatement();
            ResultSet rs = statement1.executeQuery(maxId);
            if (rs.next()) {
                idMaxim = rs.getInt("id");
            }
            idMaxim++;

            String sql = "INSERT INTO `instance`(`id`, `nume`, `fisier`, `best`) VALUES (?, ?, ?, ?)";
            PreparedStatement statement = instantaConexiune.prepareStatement(sql);

            statement.setInt(1, idMaxim);
            statement.setString(2, nume);
            FileReader input;
            input = new FileReader(caleFisier);
            statement.setClob(3, input);
            statement.setInt(4, best);

            int row = statement.executeUpdate();
            if (row > 0) {
                System.out.println("Instanta adaugata cu succes!");
            }
        } catch (SQLException ex) {
            ex.printStackTrace();
        } catch (IOException ex) {
            ex.printStackTrace();
        }
    }

    @Override
    public InstanceEntity findById(int idInstanta) {
        for (InstanceEntity instantaInLista : instanteProbleme) {
            if (instantaInLista.getId() == idInstanta) {
                return instantaInLista;
            }
        }
        return null;
    }

    @Override
    public InstanceEntity findByName(String numeInstanta) {
        for (InstanceEntity instantaInLista : instanteProbleme) {
            if (instantaInLista.getNume().equals(numeInstanta)) {
                return instantaInLista;
            }
        }
        return null;
   }
    
   public List<InstanceEntity> returnAll(){
       return this.instanteProbleme;
   }
   public static void closeConnection(){
        if(conexiuneDatabase != null){
            try {
                conexiuneDatabase.close();
            } catch (SQLException ex) {
                Logger.getLogger(ConnectionManager.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }
   
}
