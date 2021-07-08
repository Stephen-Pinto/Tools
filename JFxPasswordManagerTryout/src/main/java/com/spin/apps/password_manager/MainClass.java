package com.spin.apps.password_manager;

import com.spin.apps.password_manager.views.SimplePrivateCardForm;
import com.spin.apps.password_manager.views.prototypes.IViewEventsListener;
import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;
import javafx.stage.StageStyle;
import org.bouncycastle.jce.provider.BouncyCastleProvider;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.CommandLineRunner;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.context.properties.EnableConfigurationProperties;
import org.springframework.context.ConfigurableApplicationContext;

import java.security.Security;
import java.time.LocalDate;

//@EnableAutoConfiguration
//@Controller

@SpringBootApplication
@EnableConfigurationProperties
public class MainClass extends Application implements CommandLineRunner, IViewEventsListener {

    @Autowired
    private ConfigurableApplicationContext springContext;

    private Parent rootNode = null;

    public static void main(String[] args)
    {
        launch(MainClass.class, args);
    }

    public static void registerBouncyCastle()
    {
        Security.addProvider(new BouncyCastleProvider());
    }

    @Override
    public void run(String... args) throws Exception {
        registerBouncyCastle();
    }

    @Override
    public void start(Stage stage) throws Exception {

        try
        {
            stage.setTitle("New label");
            stage.setMaximized(false);
            //stage.setResizable(false);
            stage.setIconified(false);
            //stage.initStyle(StageStyle.TRANSPARENT);

            SimplePrivateCardForm simplePrivateCardCntrl = springContext.getBean(SimplePrivateCardForm.class);
            FXMLLoader loader = new FXMLLoader(getClass().getResource("/views/SimplePrivateCardForm.fxml"));
            loader.setController(simplePrivateCardCntrl);
            stage.setScene(new Scene(loader.load()));
            simplePrivateCardCntrl.setStage(stage);
            simplePrivateCardCntrl.Initialize();

            stage.show();
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
    }

    @Override
    public void init() throws Exception {
        springContext = SpringApplication.run(MainClass.class);
    }

    @Override
    public void actionCompleted(Object source, Object data) {
        System.out.println("Got : " + (String) data);
    }

    @Override
    public void dataChaged(Object sender, Object data) {

    }
}
