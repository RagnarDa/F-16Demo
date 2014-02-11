
#if !defined( genfilter_h )
#define       genfilter_h

#include <math.h>
class GeneralFilter
{
    private:

    template< class T > T PWRminus1( const T& x )
    {
        return ( (x % 2) ? -1 : 1);
    }
   
    public:  
		GeneralFilter( double *pdNumerators   = 0,
                       double *pdDenominators = 0,
                       int     nOrder         = 0,
                       double  dFrameTime_SEC = 0 );

        ~GeneralFilter ( void );

        void InitFilter( double *pdNumerators  ,
                         double *pdDenominators,
                         int     nOrder ,       
                         double  dFrameTime_SEC = 0 );  

        void SetCoefficients( double *pdNumerator, double *pdDenominator );

        double Filter( bool bResetFlag, double dFrameTime_SEC, double dInput );

        void SetFrameTime( double dFrameTime_SEC );

        double      m_dOutput;

    protected:

        void DeleteBuffers( void );
        void   InitP( void );
        double ProductP( int i, int j, int *pP );

        void CalculateZNumerator( void );

        void CalculateZDenominator( void );     

        int                     m_nOrder;  
        int                     m_nOrder1;

        double                  m_dFrameTime_SEC;  


        double                  m_dZDenomNormal;
        double                 *m_pdInputs;
        double                 *m_pdOutputs;
        double                 *m_pdSNumerator;
        double                 *m_pdSDenominator;
        double                 *m_pdZNumerator;
        double                 *m_pdZDenominator;
        double                 *m_pdPDt;

}; 


#endif
