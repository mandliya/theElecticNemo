struct DeleteT
{
    template<class T>  //use the template here!
    void operator()(T &x) const
    { 
      delete x;
    }
};
