





















namespace salmap_rv
{

  
  
  struct retinocentric_converter
  {
    double blurwidth_dva;
    double image_wid_dva;
    
    retinotopic_converter()
    {
    }

    
    cv::Mat create_background( const cv::Mat& input )
    {
      cv::Mat bg( cv::Size( (input.size().width*2-1), (input.size().height*2-1) ), cv::CV_8UC3, cv::Scalar(127,127,127) ); 

      return bg;
    }
    
    
    cv::Mat center_mat( const cv::Mat& input, const double eyex, const double eyey )
    {
      bool outofrange=false;
      if( eyex < 0 || eyex > 1 || eyey < 0 || eyey > 1 )
	{
	  outofrange = true;
	}

      cv::Mat fullimg = create_background( input );
      
      if( false == outofrange )
	{
	  cv::Point<double> shifted_topleft = cv::Point( 1-eyex, 1-eyey );
	  cv::Point<int> shifted_topleft_pix = shifted_topleft * input.size();

	  input.copyTo( fullimg( cv::Rect(shifted_topleft_pix, input.size() ) ) );
	}

      return fullimg;
    }
    
  };

}

