
package btun;


/** A service that is provided by a BTunClient
 */
public abstract class BTunService
{



    public abstract void initService();
    public abstract void disposeService();

    public abstract void processDataFromTunnel();
    //public abstract void processDataFromTunnel();

}

