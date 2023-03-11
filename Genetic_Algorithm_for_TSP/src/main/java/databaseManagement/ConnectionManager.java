/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package databaseManagement;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.util.Properties;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Tudor Onofrei
 */

public class ConnectionManager {
    static boolean isCreated = false;
    static Connection conexiuneDatabase = null;
    
    static Connection getConnectionManager(){
        isCreated = true;
        Properties connectionProps = new Properties();
        connectionProps.put("user", "root");
        connectionProps.put("password", "");
        
        String urlDatabase = "jdbc:mysql://localhost/tsp";
        try { 
            conexiuneDatabase = DriverManager.getConnection(urlDatabase, connectionProps);
        } catch (SQLException ex) {
            Logger.getLogger(ConnectionManager.class.getName()).log(Level.SEVERE, null, ex);
        }
        return conexiuneDatabase;
    }
    
    public static Connection getConnectionInstance(){
        if(!isCreated){
            conexiuneDatabase = getConnectionManager();
        }
        return conexiuneDatabase;
    }
    
}
