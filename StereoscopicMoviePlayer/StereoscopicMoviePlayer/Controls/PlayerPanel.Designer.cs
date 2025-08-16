namespace StereoscopicMoviePlayer
{
    partial class PlayerPanel
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
            this.ttControls = new System.Windows.Forms.ToolTip(this.components);
            this.bStop = new System.Windows.Forms.Button();
            this.bPlayPause = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // ttControls
            // 
            this.ttControls.AutoPopDelay = 5000;
            this.ttControls.InitialDelay = 500;
            this.ttControls.ReshowDelay = 100;
            // 
            // bStop
            // 
            this.bStop.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.astop;
            this.bStop.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.bStop.FlatAppearance.BorderSize = 0;
            this.bStop.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.bStop.Location = new System.Drawing.Point(29, 0);
            this.bStop.Name = "bStop";
            this.bStop.Size = new System.Drawing.Size(26, 26);
            this.bStop.TabIndex = 36;
            this.bStop.UseVisualStyleBackColor = true;
            this.bStop.Click += new System.EventHandler(this.bStop_Click);
            this.bStop.MouseEnter += new System.EventHandler(this.bStop_MouseEnter);
            // 
            // bPlayPause
            // 
            this.bPlayPause.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.aplay;
            this.bPlayPause.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.bPlayPause.FlatAppearance.BorderSize = 0;
            this.bPlayPause.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.bPlayPause.Location = new System.Drawing.Point(0, 0);
            this.bPlayPause.Name = "bPlayPause";
            this.bPlayPause.Size = new System.Drawing.Size(26, 26);
            this.bPlayPause.TabIndex = 35;
            this.bPlayPause.UseVisualStyleBackColor = true;
            this.bPlayPause.Click += new System.EventHandler(this.bPlayPause_Click);
            this.bPlayPause.MouseEnter += new System.EventHandler(this.bPlayPause_MouseEnter);
            // 
            // PlayerPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.bStop);
            this.Controls.Add(this.bPlayPause);
            this.Name = "PlayerPanel";
            this.Size = new System.Drawing.Size(55, 26);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ToolTip ttControls;
        private System.Windows.Forms.Button bStop;
        private System.Windows.Forms.Button bPlayPause;
    }
}