namespace StereoscopicMoviePlayer
{
    partial class PlayTimePanel
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.lblMovieTime = new System.Windows.Forms.Label();
            this.ttControls = new System.Windows.Forms.ToolTip(this.components);
            this.tbMovieTime = new StereoscopicMoviePlayer.SmartTrackBar();
            ((System.ComponentModel.ISupportInitialize)(this.tbMovieTime)).BeginInit();
            this.SuspendLayout();
            // 
            // lblMovieTime
            // 
            this.lblMovieTime.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.lblMovieTime.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblMovieTime.Location = new System.Drawing.Point(462, 0);
            this.lblMovieTime.Name = "lblMovieTime";
            this.lblMovieTime.Size = new System.Drawing.Size(104, 23);
            this.lblMovieTime.TabIndex = 33;
            this.lblMovieTime.Text = "00:00:00";
            this.lblMovieTime.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // ttControls
            // 
            this.ttControls.AutoPopDelay = 5000;
            this.ttControls.InitialDelay = 500;
            this.ttControls.ReshowDelay = 100;
            // 
            // tbMovieTime
            // 
            this.tbMovieTime.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tbMovieTime.AutoSize = false;
            this.tbMovieTime.Interval1 = 200;
            this.tbMovieTime.Interval2 = 400;
            this.tbMovieTime.Location = new System.Drawing.Point(0, 0);
            this.tbMovieTime.Maximum = 100;
            this.tbMovieTime.Name = "tbMovieTime";
            this.tbMovieTime.Size = new System.Drawing.Size(480, 21);
            this.tbMovieTime.TabIndex = 32;
            this.tbMovieTime.TickStyle = System.Windows.Forms.TickStyle.None;
            this.tbMovieTime.OnSmartScroll += new System.EventHandler(this.tbMovieTime_OnSmartScroll);
            this.tbMovieTime.OnValueUpdateRequest += new System.EventHandler(this.tbMovieTime_OnValueUpdateRequest);
            this.tbMovieTime.MouseEnter += new System.EventHandler(this.tbMovieTime_MouseEnter);
            // 
            // PlayTimePanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.tbMovieTime);
            this.Controls.Add(this.lblMovieTime);
            this.Name = "PlayTimePanel";
            this.Size = new System.Drawing.Size(568, 20);
            ((System.ComponentModel.ISupportInitialize)(this.tbMovieTime)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private SmartTrackBar tbMovieTime;
        private System.Windows.Forms.Label lblMovieTime;
        private System.Windows.Forms.ToolTip ttControls;
    }
}